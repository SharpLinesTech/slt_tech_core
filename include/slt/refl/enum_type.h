#ifndef SLT_REFL_ENUM_TYPE_H
#define SLT_REFL_ENUM_TYPE_H

#include "slt/refl/traits.h"
#include "slt/refl/type.h"

#define REFL_ENUM_TRAIT_BEGIN(enum_name)                    \
  template <>                                               \
  struct Traits<enum_name> : public EnumTraits<enum_name> { \
    template <typename CB>                                  \
    static void visitMembers(CB cb) {
// end of macro

#define REFL_ENUM_MEMBER(index, name, enm) cb(index, name, enm);
// end of macro

#define REFL_ENUM_TRAIT_END() \
  }                           \
  }                           \
  ;
// end of macro

namespace slt {
namespace refl {

// This expects to be inherited by a type genered by the macros at the top of
// this file.
template <typename T>
struct EnumTraits : public TraitsBase<T> {
  static slt::mem::SizeAndAlignment dataSize(T const& src,
                                             SerializationFormat fmt) {
    slt::mem::SizeAndAlignment result{0, 1};
    switch(fmt) {
      case SerializationFormat::RAW_UNVERIFIED_DATA: {
        std::underlying_type<T>::type tmp;
        result = refl::dataSize(tmp, fmt);
      } break;
      case SerializationFormat::HUMAN_READABLE: {
        bool matched = false;
        refl::Traits<T>::visitMembers(
            [&matched, &result, src](uint16_t index, const char* name, T enm) {
              if(enm == src) {
                matched = true;
                result.size = std::strlen(name);
              }
            });

        if(!matched) {
          throw EncodeError(fmt::format("Unregistered enum value: {}",
                                        static_cast<int>(src)));
        }
      } break;
    }
    return result;
  }

  static MutableDataView toData(T const& src, MutableDataView dst,
                                SerializationFormat fmt) {
    switch(fmt) {
      case SerializationFormat::RAW_UNVERIFIED_DATA: {
        using storage_type = std::underlying_type<T>::type;
        storage_type tmp = static_cast<storage_type>(src);
        dst = refl::toData(tmp, dst, fmt);
      }

      break;
      case SerializationFormat::HUMAN_READABLE:
        bool matched = false;
        refl::Traits<T>::visitMembers(
            [&matched, &dst, src](uint16_t index, const char* name, T enm) {
              if(enm == src) {
                matched = true;
                auto len = std::strlen(name);
                memcpy(dst.data(), name, len);
                dst += len;
              }
            });

        if(!matched) {
          throw EncodeError(fmt::format("Unregistered enum value: {}",
                                        static_cast<int>(src)));
        }
        break;
    }
    return dst;
  }

  static DataView assignFromData(T& dst, DataView data,
                                 SerializationFormat fmt) {
    switch(fmt) {
      case SerializationFormat::RAW_UNVERIFIED_DATA: {
        using storage_type = std::underlying_type<T>::type;
        storage_type tmp;
        data = refl::assignFromData(tmp, data, fmt);
        dst = static_cast<T>(tmp);
      } break;

      case SerializationFormat::HUMAN_READABLE: {
        bool matched = false;

        // First off, figure out whre the current identifier ends.
        auto tgt_length = 0;
        for(auto c : data) {
          if((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') ||
             (c >= '0' && c <= '9') || c == '_') {
            ++tgt_length;
          } else {
            break;
          }
        }

        StringView as_string(data.data(), tgt_length);
        data += tgt_length;
        refl::Traits<T>::visitMembers([&matched, &dst, as_string](
            uint16_t index, const char* name, T enm) {
          auto len = strlen(name);
          if(as_string == name) {
            matched = true;
            dst = enm;
          }
        });

        if(!matched) {
          throw DecodeError(fmt::format("Invalid enum value: {}", as_string));
        }
      } break;
    }
    return data;
  }
};
}
}
#endif