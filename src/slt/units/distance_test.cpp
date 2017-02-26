#include "gtest/gtest.h"

#include "slt/units/distance.h"

using namespace slt;

TEST(DistanceTest, Catch_Invalid_distance) {
  EXPECT_SLT_INVARIANT_FAILURE(Distance(-1.0f));
}

TEST(DistanceTest, Valid_positive) {
  Distance(1.0f);
}

TEST(DistanceTest, Valid_zero) {
  Distance(0.0f);
}
