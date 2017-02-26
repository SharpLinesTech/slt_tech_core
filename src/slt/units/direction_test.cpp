#include "gtest/gtest.h"

#include "slt/debug/invariant.h"
#include "slt/units/direction.h"

using namespace slt;

TEST(DirectionTest, Valid_use_as_vector) {
  Dir3 dir{0.0f, 1.0f, 0.0f};
  Vec3 val = *dir * 3.0f;
  EXPECT_EQ(3.0f, val[1]);
}

TEST(DirectionTest, Catch_Invalid_direction) {
  Vec3 v{1.0f, 0.0f, 1.0f};

  EXPECT_SLT_INVARIANT_FAILURE(Dir3(v));
}
