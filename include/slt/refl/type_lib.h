#ifndef SLT_REFL_TYPE_LIB_H
#define SLT_REFL_TYPE_LIB_H

#include <map>
#include <memory>
#include <string>

#include "slt/cpp/map_utils.h"
#include "slt/refl/reflected.h"
#include "slt/refl/traits.h"
#include "slt/refl/type.h"

namespace slt {
namespace refl {

class TypeLib {
 public:
  // Get the global TypeLib instance.
  static TypeLib& instance();

  // The the reflection information for the given type.
  template <typename T>
  Type const* get() {
    using type_impl_t = typename refl::Traits<T>::type_impl_t;
    auto key = refl::Traits<T>::typeKey();
    auto creation_cb = [this] { return std::make_unique<type_impl_t>(this); };
    return findOrInsert(types_, key, creation_cb).get();
  }

  template <typename T>
  ReflectedBuffer makeReflectedBuffer(T&& val) {
    auto const* type = get<std::remove_cv_t<std::remove_reference_t<T>>>();
    return ReflectedBuffer(type, std::forward<T>(val));
  }

  template <typename T>
  MutableReflected reflect(T* obj) {
    auto const* type = get<T>();
    return MutableReflected{type, MutableDataView(obj)};
  }

  template <typename T>
  Reflected reflect(T const* obj) {
    auto const* type = get<T>();
    return Reflected{type, DataView(obj)};
  }

 private:
  std::map<std::string, std::unique_ptr<Type>> types_;
};
}
}
#endif