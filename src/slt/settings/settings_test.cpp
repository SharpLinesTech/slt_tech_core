#include "gtest/gtest.h"

#include "slt/settings/settings.h"
#include "slt/settings/settings_internal.h"

slt::Setting<int> trivial(1, "trivial", "");

TEST(Settings, simple_setting) {
  slt::settings::_::resetAll();

  EXPECT_EQ(1, trivial.get());

  trivial.set(2);
  EXPECT_EQ(2, trivial.get());

  trivial.assign("3");
  EXPECT_EQ(3, trivial.get());

  trivial.reset();
  EXPECT_EQ(1, trivial.get());

  trivial.assign("5");
  EXPECT_EQ(5, trivial.get());
  slt::settings::_::resetAll();
  EXPECT_EQ(1, trivial.get());
}