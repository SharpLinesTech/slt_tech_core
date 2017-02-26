#ifndef SLT_SETTINGS_IMPL_H
#define SLT_SETTINGS_IMPL_H

#include "slt/refl/refl.h"
#include "spdlog/fmt/fmt.h"
#include "spdlog/fmt/ostr.h"

namespace slt {

template <typename T>
Setting<T>::Setting(T default_val, std::string name, std::string description,
                    std::function<bool(T const&)> validator = nullptr)
    : SettingBase(std::move(name), std::move(description)),
      default_(default_val),
      value_(default_val) {
  if(validator_ && !validator_(default_val)) {
    throw SettingsError("Default value does not pass the validation test");
  }
  settings::_::registerSetting(this);
}

template <typename T>
void Setting<T>::set(T const& v) {
  if(validator_ && !validator_(v)) {
    throw SettingsError("attempting to set invalid value");
  }

  value_ = v;
}

template <typename T>
T const& Setting<T>::get() const {
  return value_;
}

template <typename T>
void Setting<T>::reset() {
  value_ = default_;
}

template <typename T>
void Setting<T>::assign(StringView str) {
  slt::refl::assignFromData(value_, str.asDataView(),
                            refl::SerializationFormat::HUMAN_READABLE);
  if(validator_ && !validator_(value_)) {
    throw SettingsError(fmt::format(
        "attempting to assign invalid value: {} to {}", str, name()));
  }
}
}
#endif