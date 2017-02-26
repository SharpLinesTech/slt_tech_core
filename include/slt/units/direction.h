#ifndef SLT_UNITS_DIRECTION_H
#define SLT_UNITS_DIRECTION_H

#include "slt/debug/validated.h"

#include "slt/math/vector.h"

namespace slt {

inline void Dir2_validation(Vec2 const& v) {
  SLT_ASSERT_FLOAT_EQ(length(v), 1.0f);
}

inline void Dir3_validation(Vec3 const& v) {
  SLT_ASSERT_FLOAT_EQ(length(v), 1.0f);
}

using Dir3 = Validated<Vec3, Dir3_validation>;
using Dir2 = Validated<Vec2, Dir2_validation>;
}

#endif
