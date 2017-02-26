#ifndef SLT_DEBUG_VALIDATED_H
#define SLT_DEBUG_VALIDATED_H

#include "slt/debug/invariant.h"

namespace slt {
template <typename T>
using VALID_T = void(T const&);

// Validated types combine a value and a validating function.
// The validator function will be invoked upon construction and whenever a
// new value is assigned.
// The underlying value may only be read, using operator*(), and never
// modified without reassining an entire new value.
//
// In non-validating optimized builds, performance should be identical to
// using the underlying type directly.
template <typename T, VALID_T<T> VALIDATOR>
struct Validated {
  inline Validated(Validated const& v);
  inline Validated(Validated& v);
  inline Validated(Validated&& v);

  template <typename... ARGS_T>
  inline Validated(ARGS_T&&... a);

  inline Validated& operator=(Validated const& v);
  inline Validated& operator=(Validated&& v);

  inline Validated& operator=(T const& v);
  inline Validated& operator=(T&& v);
  inline T const& operator*() const;

 private:
  T val_;

  template <typename T, VALID_T<T> VALIDATOR>
  friend void checkInvariant(Validated<T, VALIDATOR> const& v);
};

template <typename T, VALID_T<T> VALIDATOR>
inline void checkInvariant(Validated<T, VALIDATOR> const& v);
}

#include "slt/debug/details/validated_impl.h"

#endif