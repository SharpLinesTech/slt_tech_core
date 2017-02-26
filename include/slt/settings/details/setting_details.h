#ifndef SLT_SETTINGS_SETTINGS_DETAILS_H
#define SLT_SETTINGS_SETTINGS_DETAILS_H

#include "slt/string/string_view.h"

namespace slt {
namespace settings {
namespace _ {

class SettingBase {
 public:
  SettingBase(std::string name, std::string description);
  virtual ~SettingBase();

  std::string const& name() const;
  std::string const& description() const;

  virtual void assign(StringView str) = 0;
  virtual void reset() = 0;

 private:
  std::string name_;
  std::string description_;
};

void registerSetting(SettingBase*);
}
}
}

#endif