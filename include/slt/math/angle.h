#ifndef SLT_ANGLE_H
#define SLT_ANGLE_H

#include <iostream>
#include <type_traits>

#include "slt/debug/invariant.h"
#include "slt/math/pi.h"

namespace slt {
// Angle_ts always maintain their internal representation as radians in the
// ]-PI,PI] range.
//
// Arrays and raw values:
//   A contiguous array of Angles can be interpreted as a contigous array of
// radians. This can be convenient to send Angles to a Render Program without
// having to do any memory copies.
//   IMPORTANT: Angles reinterpreted this way should only ever be read, never
// written to. This is because every Angle operation assumes that the value
// invariant is  maintained.
//
// Constructing Angles:
//   Angles are constructed through the degrees() and radians() functions.
// ex:
//  auto Angle = degrees(12.0f);

template <typename T>
class Angle_t {  // using Angle = Angle_t<float>;
  static_assert(std::is_floating_point<T>::value,
                "Only Angles of floating point types are allowed");

 public:
  T degrees() const;
  T radians() const;

  Angle_t operator-() const;

  bool operator==(Angle_t const&) const;
  bool operator!=(Angle_t const&) const;

  Angle_t operator+(Angle_t const&) const;
  Angle_t& operator+=(Angle_t const&);

  Angle_t operator-(Angle_t const&) const;
  Angle_t& operator-=(Angle_t const&);

  Angle_t operator*(T const&)const;
  Angle_t& operator*=(T const&);

  Angle_t operator/(T const&) const;
  Angle_t& operator/=(T const&);

  template <typename U>
  friend Angle_t<U> radians(U const&);

  template <typename U>
  friend Angle_t<U> degrees(U const&);

  T value_;

 private:
  Angle_t(T const&);
};

template <typename T>
Angle_t<T> radians(T const& v);

template <typename T>
Angle_t<T> degrees(T const& v);

using Angle = Angle_t<float>;
}

#include "slt/math/details/angle_impl.h"
#endif
