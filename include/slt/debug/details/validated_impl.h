#ifndef SLT_DEBUG_VALIDATED_IMPL_H
#define SLT_DEBUG_VALIDATED_IMPL_H

#include "slt/debug/invariant.h"

namespace slt {

template <typename T, VALID_T<T> VALIDATOR>
Validated<T, VALIDATOR>::Validated(Validated const& v) : val_(v.val_) {}

template <typename T, VALID_T<T> VALIDATOR>
Validated<T, VALIDATOR>::Validated(Validated& v) : val_(v.val_) {}

template <typename T, VALID_T<T> VALIDATOR>
Validated<T, VALIDATOR>::Validated(Validated&& v) : val_(std::move(v.val_)) {
  // A moved v.val_ could fail validation.
  SLT_CHECK_INVARIANT(v);
}

template <typename T, VALID_T<T> VALIDATOR>
template <typename... ARGS_T>
Validated<T, VALIDATOR>::Validated(ARGS_T&&... a)
    : val_(std::forward<ARGS_T>(a)...) {
  SLT_CHECK_INVARIANT(*this);
}

template <typename T, VALID_T<T> VALIDATOR>
Validated<T, VALIDATOR>& Validated<T, VALIDATOR>::operator=(
    Validated const& v) {
  val_ = v.val_;
  return *this;
}

template <typename T, VALID_T<T> VALIDATOR>
Validated<T, VALIDATOR>& Validated<T, VALIDATOR>::operator=(Validated&& v) {
  val_ = std::move(v.val_);
  SLT_CHECK_INVARIANT(v);
  return *this;
}

template <typename T, VALID_T<T> VALIDATOR>
Validated<T, VALIDATOR>& Validated<T, VALIDATOR>::operator=(T const& v) {
  val_ = v;
  SLT_CHECK_INVARIANT(*this);
  return *this;
}

template <typename T, VALID_T<T> VALIDATOR>
Validated<T, VALIDATOR>& Validated<T, VALIDATOR>::operator=(T&& v) {
  val_ = std::move(v);
  SLT_CHECK_INVARIANT(*this);
  return *this;
}

template <typename T, VALID_T<T> VALIDATOR>
T const& Validated<T, VALIDATOR>::operator*() const {
  return val_;
}

template <typename T, VALID_T<T> VALIDATOR>
void checkInvariant(Validated<T, VALIDATOR> const& v) {
  // If the underlying type has an invariant, it needs to be checked as well.
  SLT_CHECK_INVARIANT(v.val_);
  VALIDATOR(*v);
}
}
#endif