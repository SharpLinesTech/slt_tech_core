#ifndef SLT_REFL_REFL_H
#define SLT_REFL_REFL_H

// Unfortunately relf relies a lot on template specialization to function
// properly. Partially including refl is bound to cause major headaches. So it's
// preferable to include this file instead.

#include "slt/refl/base_type.h"
#include "slt/refl/collection_type.h"
#include "slt/refl/enum_type.h"
#include "slt/refl/string_type.h"
#include "slt/refl/struct_type.h"
#include "slt/refl/traits.h"
#include "slt/refl/type.h"

#endif