#include "slt/refl/refl.h"

namespace slt {
namespace refl {

DecodeError::DecodeError(std::string explanation)
    : std::runtime_error("Refl decoding error: " + explanation) {}

EncodeError::EncodeError(std::string explanation)
    : std::runtime_error("Refl encoding error: " + explanation) {}

Type::Type() {}
Type::~Type() {}
}
}