#ifndef SLT_MATH_VECTOR_H
#define SLT_MATH_VECTOR_H

#include "glm/glm.hpp"
#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/rotate_vector.hpp"
#include "slt/math/angle.h"
#include "slt/units/distance.h"

#include "slt/refl/refl.h"

namespace slt {
using Vec2 = glm::vec2;
using Vec3 = glm::vec3;
using Vec4 = glm::vec4;

using Mat4x4 = glm::mat4;

inline Mat4x4 perspectiveTransform(Angle vfov, float aspect_ratio,
                                   Distance near, Distance far);

namespace refl {
REFL_STRUCT_TRAIT_BEGIN(::slt::Vec2)
REFL_STRUCT_MEMBER(0, "x", &::slt::Vec2::x)
REFL_STRUCT_MEMBER(1, "y", &::slt::Vec2::y)
REFL_STRUCT_TRAIT_END()

REFL_STRUCT_TRAIT_BEGIN(::slt::Vec3)
REFL_STRUCT_MEMBER(0, "x", &::slt::Vec3::x)
REFL_STRUCT_MEMBER(1, "y", &::slt::Vec3::y)
REFL_STRUCT_MEMBER(2, "z", &::slt::Vec3::z)
REFL_STRUCT_TRAIT_END()

REFL_STRUCT_TRAIT_BEGIN(::slt::Vec4)
REFL_STRUCT_MEMBER(0, "x", &::slt::Vec4::x)
REFL_STRUCT_MEMBER(1, "y", &::slt::Vec4::y)
REFL_STRUCT_MEMBER(2, "z", &::slt::Vec4::z)
REFL_STRUCT_MEMBER(3, "w", &::slt::Vec4::w)
REFL_STRUCT_TRAIT_END()
}
}
#include "slt/math/details/vector_impl.h"

#endif
