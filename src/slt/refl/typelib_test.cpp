#include "gtest/gtest.h"

#include "slt/math/vector.h"
#include "slt/refl/base_type.h"
#include "slt/refl/collection_type.h"
#include "slt/refl/type_lib.h"

TEST(TypeLib, RegisterBaseType) {
  slt::refl::TypeLib lib;

  auto int_type = lib.get<uint8_t>();
  auto float_type = lib.get<float>();

  EXPECT_NE(nullptr, int_type);
  EXPECT_NE(nullptr, float_type);
  EXPECT_NE(int_type, float_type);
}

TEST(TypeLib, RegisterCollectionType) {
  slt::refl::TypeLib lib;

  auto vec_type = lib.get<std::vector<uint8_t>>();
  auto nested_vec_type = lib.get<std::vector<std::vector<uint8_t>>>();

  EXPECT_NE(nullptr, vec_type);
  EXPECT_NE(nullptr, nested_vec_type);
}

TEST(TypeLib, ReRegisterType) {
  slt::refl::TypeLib lib;

  // Make sure attempting to register the same type twice returns the cahced
  // copy.
  auto int_type = lib.get<uint8_t>();
  auto int_type_again = lib.get<uint8_t>();

  EXPECT_EQ(int_type, int_type_again);
}

TEST(TypeLib, RegisterVecType) {
  slt::refl::TypeLib lib;

  // Make sure attempting to register the same type twice returns the cahced
  // copy.
  auto vec_type = lib.get<slt::Vec3>();

  EXPECT_NE(nullptr, vec_type);
}

TEST(TypeLib, Reflect) {
  slt::refl::TypeLib lib;

  int v = 5;
  int c_v = 3;
  auto reflected = lib.reflect(&v);
  auto reflected_const = lib.reflect(&c_v);

  EXPECT_EQ(&v, (int*)reflected.object.data());
  EXPECT_EQ(&c_v, (int*)reflected_const.object.data());
}