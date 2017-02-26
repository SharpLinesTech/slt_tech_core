#ifndef SLT_PI_H
#define SLT_PI_H

namespace slt {
template <typename T = float>
constexpr T pi() {
  return T(3.1415926535897932385);
}

template <typename T = float>
constexpr T half_pi() {
  return pi<T>() / T(2);
}

template <typename T = float>
constexpr T two_pi() {
  return pi<T>() * T(2);
}
}
#endif
