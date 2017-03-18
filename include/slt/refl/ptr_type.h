#ifndef SLT_REFL_PTR_TYPE_H
#define SLT_REFL_PTR_TYPE_H

#include "slt/refl/traits.h"
#include "slt/refl/type.h"
#include "slt/refl/type_lib.h"
#include "spdlog/fmt/fmt.h"
#include "spdlog/fmt/ostr.h"

namespace slt {
namespace refl {

class TypeLib;

template <typename T>
class PtrType : public TypeImplCommon<T> {
 public:
  PtrType(TypeLib* lib) : TypeImplCommon<T>(lib) {
    //value_type = lib->get<typename T::value_type>();
  }

//  Type const* value_type = nullptr;
};

// This class provides basic one-size-fits-all functionality
// for container types.
template <typename T>
struct PtrTypeTraits : public TraitsBase<T> {
  using type_impl_t = PtrType<T>;

  static slt::mem::SizeAndAlignment dataSize(T const& src,
                                             SerializationFormat fmt) {
    slt::mem::SizeAndAlignment result;
    assert(false);
    return result;
  }

  static MutableDataView toData(T const& src, MutableDataView dst,
                                SerializationFormat fmt) {
    assert(false);
    return dst;
  }

  static DataView assignFromData(T& dst, DataView data,
                                 SerializationFormat fmt) {
   assert(false);
    return data;
  }
};

template <typename T>
struct Traits<T*> : public PtrTypeTraits<T> {};

}
}
#endif