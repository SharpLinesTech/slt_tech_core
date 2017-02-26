#ifndef SLT_UNITS_POSITION_H
#define SLT_UNITS_POSITION_H

#include "slt/debug/validated.h"
#include "slt/math/vector.h"

namespace slt {

inline void Pos3_validation(Vec3 const& v) {}
inline void Pos2_validation(Vec2 const& v) {}

using Pos3 = Validated<Vec3, Pos3_validation>;
using Pos2 = Validated<Vec2, Pos2_validation>;
}

#endif
