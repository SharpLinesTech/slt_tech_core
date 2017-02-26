#ifndef SLT_SETTINGS_INTERNAL_H
#define SLT_SETTINGS_INTERNAL_H

namespace slt {
namespace settings {
namespace _ {
void init(std::vector<StringView> const& argv);
// reset all registered settings to their default values.
void resetAll();
}
}
}

#endif