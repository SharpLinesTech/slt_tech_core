#ifndef SLT_REFL_COLLECTION_TYPE_H
#define SLT_REFL_COLLECTION_TYPE_H

#include <algorithm>
#include <list>
#include <vector>
#include "slt/refl/traits.h"
#include "slt/refl/type.h"
#include "slt/refl/type_lib.h"
#include "spdlog/fmt/fmt.h"
#include "spdlog/fmt/ostr.h"

namespace slt {
namespace refl {

class TypeLib;
template <typename T>
class CollectionTypeImpl : public TypeImplCommon<T> {
 public:
  CollectionTypeImpl(TypeLib* lib) : TypeImplCommon<T>(lib) {
    value_type = lib->get<typename T::value_type>();
  }

  Type const* value_type = nullptr;
};

// This class provides basic one-size-fits-all functionality
// for container types.
template <typename T>
struct CollectionTraits : public TraitsBase<T> {
  using type_impl_t = CollectionTypeImpl<T>;

  static slt::mem::SizeAndAlignment dataSize(T const& src,
                                             SerializationFormat fmt) {
    slt::mem::SizeAndAlignment result;
    switch(fmt) {
      case SerializationFormat::RAW_UNVERIFIED_DATA: {
        result = refl::dataSize(*(uint32_t*)nullptr, fmt);

        for(auto const& m : src) {
          auto tmp = refl::dataSize(m, fmt);
          result.size = mem::alignSize(result.size, tmp.align);
          result.size += tmp.size;
        }
      } break;
      case SerializationFormat::HUMAN_READABLE:
        result.align = 1;
        result.size = 2;  // for the two brackets
        result.size +=
            std::max(std::size_t(1), src.size()) - 1;  // space between values
        for(auto const& m : src) {
          auto tmp = refl::dataSize(m, fmt);
          result.size += tmp.size;
        }
        break;
    }

    return result;
  }

  static MutableDataView toData(T const& src, MutableDataView dst,
                                SerializationFormat fmt) {
    switch(fmt) {
      case SerializationFormat::RAW_UNVERIFIED_DATA: {
        uint32_t size = uint32_t(src.size());
        dst = refl::toData(size, dst, fmt);
        for(auto const& m : src) {
          dst = refl::toData(m, dst, fmt);
        }
      } break;
      case SerializationFormat::HUMAN_READABLE: {
        *dst.data() = '[';
        dst += 1;
        bool first = true;
        for(auto const& m : src) {
          if(first) {
            first = false;
          } else {
            *dst.data() = ' ';
            dst += 1;
          }
          dst = refl::toData(m, dst, fmt);
        }
        *dst.data() = ']';
        dst += 1;
      } break;
    }

    return dst;
  }

  static DataView assignFromData(T& dst, DataView data,
                                 SerializationFormat fmt) {
    dst = T();
    switch(fmt) {
      case SerializationFormat::RAW_UNVERIFIED_DATA: {
        uint32_t size;
        data = refl::assignFromData(size, data, fmt);
        for(uint32_t i = 0; i < size; ++i) {
          typename T::value_type tmp;
          data = refl::assignFromData(tmp, data, fmt);
          dst.emplace_back(std::move(tmp));
        }
      } break;
      case SerializationFormat::HUMAN_READABLE: {
        StringView as_string = data;
        as_string = slt::lstrip(as_string);
        if(!startsWith(as_string, '[') || as_string.empty()) {
          throw DecodeError(fmt::format("Expecting '[' at: {}", as_string));
        }

        ++as_string;

        while(1) {
          as_string = slt::lstrip(as_string);
          if(as_string.empty()) {
            throw DecodeError(
                "Reached end of buffer unexpectedly (open square bracket)");
          }

          if(startsWith(as_string, ']')) {
            ++as_string;
            break;
          }

          typename T::value_type tmp;
          as_string = refl::assignFromData(tmp, as_string.asDataView(), fmt);
          dst.emplace_back(std::move(tmp));
        }

        data = as_string.asDataView();
      } break;
    }

    return data;
  }
};

template <typename T>
struct Traits<std::list<T>> : public CollectionTraits<std::list<T>> {};

// Vector is special, If we can simply memcopy the contents, we might a well.
template <typename T>
struct Traits<std::vector<T>,
              typename std::enable_if<!CanBeMemCopied<T>::value>::type>
    : public CollectionTraits<std::vector<T>> {};

// Vectors of trivially copyable types get a special implementation.
template <typename T>
struct Traits<std::vector<T>,
              typename std::enable_if<CanBeMemCopied<T>::value>::type>
    : public CollectionTraits<std::vector<T>> {
  static slt::mem::SizeAndAlignment dataSize(std::vector<T> const& src,
                                             SerializationFormat fmt) {
    slt::mem::SizeAndAlignment result;
    switch(fmt) {
      case SerializationFormat::RAW_UNVERIFIED_DATA: {
        result = refl::dataSize(*(uint32_t*)nullptr, fmt);
        result.size = mem::alignSize(result.size, alignof(T));
        result.size += src.size() * sizeof(T);
      } break;
      default:
        result = CollectionTraits<std::vector<T>>::dataSize(src, fmt);
        break;
    }

    return result;
  }

  static MutableDataView toData(std::vector<T> const& src, MutableDataView dst,
                                SerializationFormat fmt) {
    switch(fmt) {
      case SerializationFormat::RAW_UNVERIFIED_DATA: {
        uint32_t size = uint32_t(src.size());
        dst = refl::toData(size, dst, fmt);
        dst = dst.align(alignof(T));
        SLT_ASSERT_GE(dst.size(), sizeof(T) * src.size());
        memcpy(dst.data(), src.data(), sizeof(T) * src.size());
        dst += sizeof(T) * src.size();
      } break;
      default:
        dst = CollectionTraits<std::vector<T>>::toData(src, dst, fmt);
        break;
    }

    return dst;
  }

  static DataView assignFromData(std::vector<T>& dst, DataView data,
                                 SerializationFormat fmt) {
    switch(fmt) {
      case SerializationFormat::RAW_UNVERIFIED_DATA: {
        uint32_t size;
        data = refl::assignFromData(size, data, fmt);
        data = data.align(alignof(T));
        dst.resize(size);
        memcpy(dst.data(), data.data(), size * sizeof(T));
        data += sizeof(T) * size;
      } break;
      default:
        data = CollectionTraits<std::vector<T>>::assignFromData(dst, data, fmt);
        break;
    }

    return data;
  }
};
}
}
#endif