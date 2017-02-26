#ifndef SLT_MATH_VECTOR_IMPL_H
#define SLT_MATH_VECTOR_IMPL_H

#include "glm/gtc/matrix_transform.hpp"

namespace slt {
Mat4x4 perspectiveTransform(Angle vfov, float aspect_ratio, Distance near_p,
                            Distance far_p) {
  return glm::perspective(vfov.radians(), aspect_ratio, *near_p, *far_p);
}
}
#endif