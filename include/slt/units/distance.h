#ifndef SLT_UNITS_DISTANCE_H
#define SLT_UNITS_DISTANCE_H

#include "slt/debug/validated.h"

namespace slt {
inline void Distance_validation(float const& v) {
  SLT_ASSERT_GE(v, 0.0f);
}

using Distance = Validated<float, Distance_validation>;
}

#endif
