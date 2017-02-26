#include "gtest/gtest.h"

#include "slt/debug/validated.h"

void validator(int const& v) {
  // only accept 5 as a value
  SLT_ASSERT_LE(v, 5);
}

using TestValidated = slt::Validated<int, validator>;

TEST(ValidatedTest, Passes) {
  TestValidated x(5);

  EXPECT_EQ(*x, 5);
}

TEST(ValidatedTest, Fails) {
  EXPECT_SLT_INVARIANT_FAILURE(TestValidated(12));
}

TEST(ValidatedTest, VectorFails) {
  EXPECT_SLT_INVARIANT_FAILURE(std::vector<TestValidated>({5, 5, 12, 5, 5}));
}

TEST(ValidatedTest, CopyAndMove) {
  TestValidated x(5);
  TestValidated y = x;
  TestValidated z = std::move(x);
}

TEST(ValidatedTest, Assign) {
  TestValidated x(5);
  TestValidated y(2);
  TestValidated z(1);

  y = x;
  z = std::move(x);

  EXPECT_EQ(*y, 5);
  EXPECT_EQ(*z, 5);
}