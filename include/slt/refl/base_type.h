#ifndef SLT_REFL_BASE_TYPE_H
#define SLT_REFL_BASE_TYPE_H

#include "slt/refl/traits.h"
#include "slt/refl/type.h"
#include "slt/string/string_view.h"

#include <sstream>

namespace slt {
namespace refl {

class TypeLib;
class Type;

template <typename T>
class BaseTypeImpl : public TypeImplCommon<T> {
 public:
  BaseTypeImpl(TypeLib* lib) : TypeImplCommon<T>(lib) {}
};

template <typename T>
struct BaseTypeTraits : public TraitsBase<T> {
  using type_impl_t = BaseTypeImpl<T>;

  static slt::mem::SizeAndAlignment dataSize(T const& src,
                                             SerializationFormat fmt) {
    switch(fmt) {
      case SerializationFormat::RAW_UNVERIFIED_DATA:
        return {sizeof(T), alignof(T)};
      case SerializationFormat::HUMAN_READABLE:
        return {std::to_string(src).length(), 1};
    }

    return {0, 1};
  }

  static MutableDataView toData(T const& src, MutableDataView dst,
                                SerializationFormat fmt) {
    switch(fmt) {
      case SerializationFormat::RAW_UNVERIFIED_DATA:
        dst = dst.align(alignof(T));
        *dst.as<T>() = src;
        dst += sizeof(T);
        break;
      case SerializationFormat::HUMAN_READABLE: {
        auto as_string = std::to_string(src);
        std::memcpy(dst.data(), as_string.data(), as_string.length());
        dst += as_string.length();
      } break;
    }

    return dst;
  }

  static DataView assignFromData(T& dst, DataView data,
                                 SerializationFormat fmt) {
    switch(fmt) {
      case SerializationFormat::RAW_UNVERIFIED_DATA:
        data = data.align(alignof(T));
        dst = *data.as<T>();
        data += sizeof(T);
        break;
      case SerializationFormat::HUMAN_READABLE: {
        auto as_string = StringView(data).toString();
        std::istringstream stream(as_string);
        stream >> dst;
        auto stream_pos = stream.tellg();
        if(stream_pos == std::istringstream::pos_type(-1)) {
          data = DataView();
        } else {
          data += stream_pos;
        }
      } break;
    }
    return data;
  }
};

template <>
struct Traits<bool> : public BaseTypeTraits<bool> {};
template <>
struct Traits<int8_t> : public BaseTypeTraits<int8_t> {};
template <>
struct Traits<uint8_t> : public BaseTypeTraits<uint8_t> {};
template <>
struct Traits<int16_t> : public BaseTypeTraits<int16_t> {};
template <>
struct Traits<uint16_t> : public BaseTypeTraits<uint16_t> {};
template <>
struct Traits<int32_t> : public BaseTypeTraits<int32_t> {};
template <>
struct Traits<uint32_t> : public BaseTypeTraits<uint32_t> {};
template <>
struct Traits<int64_t> : public BaseTypeTraits<int64_t> {};
template <>
struct Traits<uint64_t> : public BaseTypeTraits<uint64_t> {};
template <>
struct Traits<float> : public BaseTypeTraits<float> {};
template <>
struct Traits<double> : public BaseTypeTraits<double> {};
}
}

#endif