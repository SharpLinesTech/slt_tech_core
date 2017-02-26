#include "slt/settings/settings.h"
#include "slt/log/log.h"
#include "slt/log/log_internal.h"
#include "slt/settings/settings_internal.h"
#include "slt/string/split.h"

#include <iostream>
#include <map>
#include <string>

namespace {
using settings_store_t =
    std::map<std::string, ::slt::settings::_::SettingBase*>;
settings_store_t& settingsStore() {
  static settings_store_t instance;
  return instance;
}
}

namespace slt {

SettingsError::SettingsError(std::string const& msg)
    : std::runtime_error(msg) {}

namespace settings {
namespace _ {

SettingBase::SettingBase(std::string name, std::string desc)
    : name_(std::move(name)), description_(std::move(desc)) {}

SettingBase::~SettingBase() {}

std::string const& SettingBase::description() const {
  return description_;
}

std::string const& SettingBase::name() const {
  return name_;
}

void init(std::vector<StringView> const& args) {
  auto& store = settingsStore();

  logging::init_log->info("Begin args:");
  for(auto arg : args) {
    logging::init_log->info("arg: {}", arg);

    if(startsWith(arg, "--")) {
      arg = arg.substr(2);
      auto parts = split(arg, '=');
      if(parts.size() == 2) {
        auto found = store.find(parts[0].toString());
        if(found == store.end()) {
          throw SettingsError(fmt::format("unknown setting: {}", arg));
        } else {
          found->second->assign(parts[1]);
        }
      } else {
        throw SettingsError(fmt::format("Badly formatted setting: {}", arg));
      }
    } else {
      throw SettingsError(fmt::format("Badly formatted setting: {}", arg));
    }
  }
  logging::init_log->info("Done parsing args");
}

void resetAll() {
  for(auto& setting : settingsStore()) {
    setting.second->reset();
  }
}

void registerSetting(SettingBase* setting) {
  auto& store = settingsStore();
  auto const& name_str = setting->name();

  if(store.find(name_str) != store.end()) {
    throw SettingsError(
        fmt::format("trying to register the same setting twice: {}", name_str));
  }

  store[name_str] = setting;
}
}
}
}