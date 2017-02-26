#ifndef SLT_REFL_STRUCT_TYPE_H
#define SLT_REFL_STRUCT_TYPE_H

#include "slt/refl/traits.h"
#include "slt/refl/type.h"
#include "slt/refl/type_lib.h"

#include "spdlog/fmt/fmt.h"
#include "spdlog/fmt/ostr.h"

#define REFL_STRUCT_TRAIT_BEGIN(struct_name)                            \
  template <>                                                           \
  struct Traits<struct_name, void> : public StructTraits<struct_name> { \
    template <typename CB>                                              \
    static void visitMembers(CB cb) {  \
//end of macro

#define REFL_STRUCT_MEMBER(index, name, member) \
  cb(index, name, member);  \
// end of macro

#define REFL_STRUCT_TRAIT_END() \
  }                             \
  }                             \
  ;

namespace slt {
namespace refl {

// This expects to be inherited by a type genered by the macros at the top of
// this file.
template <typename T>
struct StructTraits : public TraitsBase<T> {
  using type_impl_t = TypeImplCommon<T>;
  static slt::mem::SizeAndAlignment dataSize(T const& src,
                                             SerializationFormat fmt) {
    slt::mem::SizeAndAlignment result{0, 1};
    switch(fmt) {
      case SerializationFormat::RAW_UNVERIFIED_DATA:
        refl::Traits<T>::visitMembers([&result, &src, fmt](
            uint16_t index, const char* name, auto member) {
          auto tmp = slt::refl::dataSize(src.*member, fmt);
          if(result.size == 0) {
            result = tmp;
          } else {
            result.size = mem::alignSize(result.size, tmp.align);
            result.size += tmp.size;
            result.align = std::max(result.align, tmp.align);
          }
        });
        break;
      case SerializationFormat::HUMAN_READABLE:
        refl::Traits<T>::visitMembers([&result, &src, fmt](
            uint16_t index, const char* name, auto member) {
          if(result.size > 0) {
            result.size += 1;  // For the space between values.
          }
          auto tmp = slt::refl::dataSize(src.*member, fmt);
          result.size += tmp.size;
          result.size += 2 + strlen(name);
        });

        result.size += 2;  // For the two curly braces.
        break;
    }
    return result;
  }

  static MutableDataView toData(T const& src, MutableDataView dst,
                                SerializationFormat fmt) {
    switch(fmt) {
      case SerializationFormat::RAW_UNVERIFIED_DATA: {
        refl::Traits<T>::visitMembers(
            [&src, &dst, fmt](uint16_t index, const char* name, auto member) {
              dst = refl::toData(src.*member, dst, fmt);
            });
      } break;
      case SerializationFormat::HUMAN_READABLE: {
        *dst.data() = '{';
        dst += 1;
        bool first = true;

        refl::Traits<T>::visitMembers([&src, &dst, &first, fmt](
            uint16_t index, const char* name, auto member) {
          if(first) {
            first = false;
          } else {
            *dst.data() = ' ';
            dst += 1;
          }
          auto name_len = strlen(name);
          memcpy(dst.data(), name, name_len);
          dst += name_len;
          *dst.data() = ':';
          dst += 1;
          *dst.data() = ' ';
          dst += 1;
          dst = refl::toData(src.*member, dst, fmt);
        });

        *dst.data() = '}';
        dst += 1;
      } break;
    }
    return dst;
  }

  static DataView assignFromData(T& dst, DataView data,
                                 SerializationFormat fmt) {
    switch(fmt) {
      case SerializationFormat::RAW_UNVERIFIED_DATA: {
        refl::Traits<T>::visitMembers(
            [&dst, &data, fmt](uint16_t index, const char* name, auto member) {
              data = refl::assignFromData(dst.*member, data, fmt);
            });
      } break;

      case SerializationFormat::HUMAN_READABLE: {
        StringView as_string = data;
        as_string = slt::lstrip(as_string);

        if(!startsWith(as_string, '{') || as_string.empty()) {
          throw DecodeError(fmt::format("Expecting '{' at: {}", as_string));
        }
        ++as_string;

        while(1) {
          as_string = slt::lstrip(as_string);

          if(as_string.empty()) {
            throw DecodeError(
                "Reached end of buffer unexpectedly (open curly brace)");
          }

          if(startsWith(as_string, '}')) {
            ++as_string;
            break;
          }

          auto splitted = find(as_string, ':');
          auto member_name = splitted.first;
          if(member_name.empty()) {
            throw DecodeError(
                fmt::format("Expecting identifier at: {}", as_string));
          }
          as_string = slt::lstrip(splitted.second);
          // Very lazy and kinda inneficient, but so much simpler.
          bool found = false;
          Traits<T>::visitMembers([&found, &as_string, &dst, member_name, fmt](
              uint16_t index, const char* name, auto member) {
            if(member_name == name) {
              found = true;
              as_string = refl::assignFromData(dst.*member,
                                               as_string.asDataView(), fmt);
            }
          });

          if(!found) {
            throw DecodeError(
                fmt::format("Unrecognized identifier: {}", member_name));
          }
        }

        data = as_string.asDataView();
      } break;
    }
    return data;
  }
};
}
}
#endif
