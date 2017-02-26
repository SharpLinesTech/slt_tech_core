#ifndef SLT_MATH_ANGLE_IMPL_H
#define SLT_MATH_ANGLE_IMPL_H

namespace slt {

template <typename T>
Angle_t<T>::Angle_t(T const& v) : value_(v) {
  SLT_CHECK_INVARIANT(*this);
}

template <typename T>
T Angle_t<T>::degrees() const {
  return value_ * T(180) / pi<T>();
}

template <typename T>
T Angle_t<T>::radians() const {
  return value_;
}

template <typename T>
Angle_t<T> Angle_t<T>::operator-() const {
  // Special case, we try to keep positive pi.
  return Angle_t<T>(value_ == pi<T>() ? value_ : -value_);
}

template <typename T>
bool Angle_t<T>::operator==(Angle_t const& rhs) const {
  return value_ == rhs.value_;
}

template <typename T>
Angle_t<T> Angle_t<T>::operator+(Angle_t<T> const& rhs) const {
  auto result = *this;
  result += rhs;
  return result;
}

template <typename T>
Angle_t<T>& Angle_t<T>::operator+=(Angle_t<T> const& rhs) {
  value_ += rhs.value_;
  if(value_ > pi<T>()) {
    value_ -= two_pi<T>();
  } else if(value_ <= -pi<T>()) {
    value_ += two_pi<T>();
  }
  SLT_CHECK_INVARIANT(*this);
  return *this;
}

template <typename T>
Angle_t<T> Angle_t<T>::operator-(Angle_t<T> const& rhs) const {
  auto result = *this;
  result -= rhs;
  return result;
}

template <typename T>
Angle_t<T>& Angle_t<T>::operator-=(Angle_t<T> const& rhs) {
  value_ -= rhs.value_;
  if(value_ > pi<T>()) {
    value_ -= two_pi<T>();
  } else if(value_ <= -pi<T>()) {
    value_ += two_pi<T>();
  }
  SLT_CHECK_INVARIANT(*this);
  return *this;
}

template <typename T>
Angle_t<T>& Angle_t<T>::operator*=(T const& rhs) {
  value_ *= rhs;
  while(value_ > pi<T>()) {
    value_ -= two_pi<T>();
  }

  while(value_ <= -pi<T>()) {
    value_ += two_pi<T>();
  }
  SLT_CHECK_INVARIANT(*this);
  return *this;
}

template <typename T>
Angle_t<T> Angle_t<T>::operator*(T const& rhs) const {
  Angle_t<T> result = *this;
  result *= rhs;
  return result;
}

template <typename T>
Angle_t<T>& Angle_t<T>::operator/=(T const& rhs) {
  value_ /= rhs;
  while(value_ > pi<T>()) {
    value_ -= two_pi<T>();
  }

  while(value_ <= -pi<T>()) {
    value_ += two_pi<T>();
  }
  SLT_CHECK_INVARIANT(*this);
  return *this;
}

template <typename T>
Angle_t<T> Angle_t<T>::operator/(T const& rhs) const {
  Angle_t<T> result = *this;
  result /= rhs;
  return result;
}

template <typename T>
Angle_t<T> operator*(T const& lhs, Angle_t<T> const& rhs) {
  return rhs * lhs;
}

template <typename T>
inline bool operator<(Angle_t<T> const& lhs, Angle_t<T> const& rhs) {
  return lhs.value_ < rhs.value_;
}

template <typename T>
inline bool operator>(Angle_t<T> const& lhs, Angle_t<T> const& rhs) {
  return lhs.value_ > rhs.value_;
}

template <typename T>
Angle_t<T> radians(T const& v) {
  T tmp = v;
  while(tmp > pi<T>()) {
    tmp -= two_pi<T>();
  }

  while(tmp <= -pi<T>()) {
    tmp += two_pi<T>();
  }
  return Angle_t<T>(tmp);
}

template <typename T>
Angle_t<T> degrees(T const& v) {
  T tmp = v;
  while(tmp > T(180)) {
    tmp -= T(360);
  }

  while(tmp <= T(-180)) {
    tmp += T(360);
  }

  return Angle_t<T>(tmp / T(180) * pi<T>());
}

template <typename T>
void checkInvariant(Angle_t<T> const& a) {
  SLT_ASSERT(a.value_ <= pi<T>());
  SLT_ASSERT(a.value_ > -pi<T>());
}

template <typename T>
std::ostream& operator<<(std::ostream& stream, Angle_t<T> const& angle) {
  stream << angle.degrees() << "°";
  return stream;
}
}

#endif