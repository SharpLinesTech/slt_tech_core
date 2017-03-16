#ifndef SLT_CORE_H
#define SLT_CORE_H

#include <vector>

#include "slt/concur/event_queue.h"
#include "slt/string/string_view.h"

namespace slt {

struct Core {
  Core();
  Core(int argc, const char* argv[]);
  Core(std::vector<StringView> const&);
  ~Core();

  void update();

  slt::concur::EventQueue main_queue;

  static Core* instance;
};
}
#endif