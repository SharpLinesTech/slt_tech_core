#ifndef SLT_SETTINGS_SETTINGS_H
#define SLT_SETTINGS_SETTINGS_H

#include <functional>
#include "slt/settings/details/setting_details.h"
#include "slt/string/string_view.h"

namespace slt {

// Error Thrown when something goes wrong will seting up or processing Settings.
struct SettingsError : public std::runtime_error {
  SettingsError(std::string const&);
};

// Settings are self-registering global properties that will be set when the slt
// Core is being initialized. 
//
// Settings should always be defined as global variables.
// Settings will have their default until the core has been initialized, and
// after the core has shut down.
template <typename T>
class Setting : public settings::_::SettingBase {
 public:
  using Validator = std::function<bool(T const&)>;

  Setting(T default_val, std::string name, std::string description,
          Validator validator = nullptr);

  // REtrieve the value of the setting.
  T const& get() const;

  // Sets the value of the setting.
  void set(T const& v);

  // Set the value of the setting using a string representation of the value.
  void assign(StringView str) override;

  // Resets the setting to whatever its default is.
  void reset();

 private:
  T default_;
  T value_;
  Validator validator_;
};
}

#include "slt/settings/details/setting_impl.h"

#endif
