#include "slt/refl/type_lib.h"

namespace slt {
namespace refl {
TypeLib& TypeLib::instance() {
  static TypeLib singleton;
  return singleton;
}
}
}