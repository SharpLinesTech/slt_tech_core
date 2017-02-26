#ifndef SLT_REFL_STRING_TYPE_H
#define SLT_REFL_STRING_TYPE_H

#include <string>
#include "slt/refl/traits.h"

namespace slt {
namespace refl {

template <>
struct Traits<std::string> : public TraitsBase<std::string> {
  using type_impl_t = TypeImplCommon<std::string>;
  static slt::mem::SizeAndAlignment dataSize(std::string const& src,
                                             SerializationFormat fmt) {
    switch(fmt) {
      case SerializationFormat::RAW_UNVERIFIED_DATA: {
        auto result = refl::dataSize(*(uint32_t*)nullptr, fmt);
        result.size += src.length();
        return result;
      }
      case SerializationFormat::HUMAN_READABLE:
        return {src.length() + 2, 1};
    }

    return {0, 1};
  }

  static MutableDataView toData(std::string const& src, MutableDataView dst,
                                SerializationFormat fmt) {
    switch(fmt) {
      case SerializationFormat::RAW_UNVERIFIED_DATA: {
        uint32_t len = uint32_t(src.length());
        dst = refl::toData(len, dst, fmt);
        memcpy(dst.data(), src.data(), len);
        dst += len;
      } break;

      case SerializationFormat::HUMAN_READABLE: {
        // TODO: escape the string.
        *dst.data() = '"';
        dst += 1;
        std::memcpy(dst.data(), src.data(), src.length());
        dst += src.length();
        *dst.data() = '"';
        dst += 1;

      } break;
    }
    return dst;
  }

  static DataView assignFromData(std::string& dst, DataView data,
                                 SerializationFormat fmt) {
    switch(fmt) {
      case SerializationFormat::RAW_UNVERIFIED_DATA: {
        uint32_t len = 0;
        data = refl::assignFromData(len, data, fmt);
        dst = std::string(data.data(), len);
        data += len;
      } break;
      case SerializationFormat::HUMAN_READABLE: {
        StringView as_string = data;
        if(as_string.length() < 2 || !startsWith(as_string, '"')) {
          // TODO: This isn't great... we could confuse badly formatted data
          // with a quoteless string.
          dst = as_string.toString();
        } else {
          ++as_string;
          auto end_found = find(as_string, '"');
          as_string = end_found.first;
          data = end_found.second.asDataView();
          dst = as_string.toString();
        }
      } break;
    }
    return data;
  }
};
}
}

#endif