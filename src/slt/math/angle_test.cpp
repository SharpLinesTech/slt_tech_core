#include "slt/math/angle.h"
#include "gtest/gtest.h"

using namespace slt;

TEST(AngleTest, looping_behavior) {
  // pi and -pi are equivalent
  EXPECT_EQ(radians(pi()), radians(-pi()));
  EXPECT_EQ(degrees(180.0f), degrees(-180.0f));

  // pi is positive
  EXPECT_GT(radians(pi()).radians(), 0.0f);
  EXPECT_GT(degrees(180.0f).radians(), 0.0f);

  //-pi is positive
  EXPECT_GT(radians(-pi()).radians(), 0.0f);
  EXPECT_GT(degrees(-180.0f).radians(), 0.0f);

  // The floating point number immediately after pi should loop around
  auto next_after_pi = radians(std::nextafter(pi(), 100.0f));
  EXPECT_LT(next_after_pi.radians(), 0.0f);
  EXPECT_GT(next_after_pi.radians(), -pi());

  next_after_pi = degrees(std::nextafter(180.0f, 360.0f));
  EXPECT_LT(next_after_pi.degrees(), 0.0f);
  EXPECT_GT(next_after_pi.degrees(), -180.0f);

  // The floating point number immediately before minus pi should loop around
  auto next_before_minus_pi = radians(std::nextafter(-pi(), -100.0f));
  EXPECT_GT(next_before_minus_pi.radians(), 0.0f);
  EXPECT_LT(next_before_minus_pi.radians(), pi());

  next_before_minus_pi = degrees(std::nextafter(-180.0f, -300.0f));
  EXPECT_GT(next_before_minus_pi.degrees(), 0.0f);
  EXPECT_LT(next_before_minus_pi.degrees(), 180.0f);

  // a perfect loop should be identical to 0, regardless of internal
  // representation.
  EXPECT_EQ(0.0f, radians(two_pi()).radians());
  EXPECT_EQ(0.0f, degrees(360.0f).radians());

  // positive turn stays the same
  EXPECT_EQ(pi(), radians(pi()).radians());
  EXPECT_FLOAT_EQ(180.0f, degrees(180.0f).degrees());

  // negative half turn stays the gets flipped
  EXPECT_FLOAT_EQ(pi(), radians(-pi()).radians());
  EXPECT_FLOAT_EQ(180.0f, degrees(-180.0f).degrees());
}

TEST(AngleTest, multi_loop) {
  EXPECT_EQ(degrees(120.0f), degrees(120.0f + 360.0f * 10.0f));
  EXPECT_EQ(degrees(120.0f), degrees(120.0f - 360.0f * 10.0f));
  EXPECT_EQ(degrees(-120.0f), degrees(-120.0f + 360.0f * 10.0f));
  EXPECT_EQ(degrees(-120.0f), degrees(-120.0f - 360.0f * 10.0f));
}

TEST(AngleTest, radians_to_degrees) {
  // Zero should stay EXACTLY zero
  EXPECT_EQ(0.0f, radians(0.0f).degrees());

  EXPECT_FLOAT_EQ(90.0f, radians(half_pi()).degrees());
  EXPECT_FLOAT_EQ(-90.0f, radians(-half_pi()).degrees());
}

TEST(AngleTest, degrees_to_radians) {
  // Zero should stay EXACTLY zero
  EXPECT_EQ(0.0f, degrees(0.0f).radians());

  EXPECT_FLOAT_EQ(half_pi(), degrees(90.0f).radians());
  EXPECT_FLOAT_EQ(-half_pi(), degrees(-90.0f).radians());
}

TEST(AngleTest, inverting) {
  EXPECT_EQ(radians(0.0f), -radians(0.0f));

  EXPECT_EQ(degrees(180.0f), -degrees(180.0f));
  EXPECT_EQ(degrees(180.0f), degrees(-180.0f));
  EXPECT_EQ(-degrees(180.0f), -degrees(-180.0f));
}

TEST(AngleTest, scaling) {
  EXPECT_EQ(degrees(-90.0f) * 2.0f, degrees(180.0f));
  EXPECT_EQ(degrees(90.0f) * 2.0f, degrees(180.0f));
  EXPECT_EQ(2.0f * degrees(90.0f), degrees(180.0f));
  EXPECT_EQ(2.0f * degrees(180.0f), degrees(0.0f));

  EXPECT_EQ(degrees(-90.0f) / 2.0f, degrees(-45.0f));
  EXPECT_EQ(degrees(90.0f) / 2.0f, degrees(45.0f));
}

TEST(AngleTest, adding) {
  // a perfect loop should be identical to 0, regardless of internal
  // representation.
  auto quarter = degrees(90.0f);
  auto half = degrees(180.0f);
  auto three_quarter = degrees(270.0f);
  auto full = radians(0.0f);

  EXPECT_FLOAT_EQ(half.radians(), (quarter + quarter).radians());
  EXPECT_FLOAT_EQ(full.radians(), (half + half).radians());
  EXPECT_FLOAT_EQ(full.radians(), (quarter + three_quarter).radians());
  EXPECT_FLOAT_EQ(three_quarter.radians(), (half + quarter).radians());

  auto tmp = quarter;
  tmp += half;
  EXPECT_FLOAT_EQ(three_quarter.radians(), tmp.radians());
}

TEST(AngleTest, subbing) {
  // A perfect loop should be identical to 0, regardless of internal
  // representation.
  auto quarter = degrees(90.0f);
  auto half = degrees(180.0f);
  auto three_quarter = degrees(270.0f);
  auto full = radians(0.0f);

  EXPECT_FLOAT_EQ(full.radians(), (quarter - quarter).radians());
  EXPECT_FLOAT_EQ(quarter.radians(), (half - quarter).radians());
  EXPECT_FLOAT_EQ(half.radians(), (quarter - three_quarter).radians());

  auto tmp = quarter;
  tmp -= half;
  EXPECT_FLOAT_EQ(-quarter.radians(), tmp.radians());
}
