#include "gtest/gtest.h"

#include "slt/debug/invariant.h"

struct MyTypeWithoutInvariant {
  int value;
};

struct MyTypeWithInvariant {
  int value;
};

namespace slt {
template <>
void checkInvariant(MyTypeWithInvariant const& v) {
  // If the underlying type has an invariant, it needs to be checked as well.
  SLT_ASSERT_GT(v.value, 0);
}
}

TEST(InvariantTest, Test_validated_type) {
  MyTypeWithInvariant pass{3};
  MyTypeWithInvariant fail{0};

  EXPECT_SLT_INVARIANT_FAILURE(SLT_CHECK_INVARIANT(fail));
  SLT_CHECK_INVARIANT(pass);
}

TEST(InvariantTest, Test_non_validated_type) {
  MyTypeWithoutInvariant pass{3};
  MyTypeWithoutInvariant fail{0};

  SLT_CHECK_INVARIANT(fail);
  SLT_CHECK_INVARIANT(pass);
}