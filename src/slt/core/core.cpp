#include "slt/core/core.h"
#include "slt/file/file_internal.h"
#include "slt/log/log_internal.h"
#include "slt/settings/settings_internal.h"

namespace {
std::vector<slt::StringView> argvToArgs(int argc, const char* argv[]) {
  std::vector<slt::StringView> args(argc - 1);
  for(int i = 1; i < argc; ++i) {
    args[i - 1] = argv[i];
  }
  return args;
}
}
namespace slt {

Core* Core::instance = nullptr;

Core::Core() : Core(std::vector<StringView>()) {}

Core::Core(int argc, const char* argv[]) : Core(argvToArgs(argc, argv)) {}

Core::Core(std::vector<StringView> const& args) {
  // There should only ever be a single core instance in existence.
  SLT_ASSERT(instance == nullptr);
  instance = this;
  logging::preInit();

  settings::_::init(args);
  logging::init();

  file::startFilesystemThread();
}

Core::~Core() {
  file::stopFilesystemThread();
  instance = nullptr;

  settings::_::resetAll();
  logging::shutdown();
}

void Core::update() {
  main_queue.executeAll();
}
}
