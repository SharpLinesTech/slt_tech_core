#include "gtest/gtest.h"

#include "slt/core/core.h"
#include "slt/settings/settings.h"

slt::Setting<int> trivial(1, "trivial", "");

TEST(Settings, parse_argv) {
  const char* argv[] = {"", "--trivial=3"};

  {
    slt::Core core(2, argv);
    EXPECT_EQ(3, trivial.get());
  }

  // Setting should be back to normal
  EXPECT_EQ(1, trivial.get());
}

TEST(Settings, invalid_setting) {
  const char* argv[] = {"", "--unknown=3"};

  EXPECT_THROW(slt::Core(2, argv), slt::SettingsError);
}