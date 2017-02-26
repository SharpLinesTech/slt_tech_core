SET(SLT_DEBUG_PRIVATE
  assert.cpp
)

SET(SLT_DEBUG_PUBLIC
  ${SLT_TECH_PUBLIC_INCLUDE}/slt/debug/assert.h
  ${SLT_TECH_PUBLIC_INCLUDE}/slt/debug/invariant.h
  ${SLT_TECH_PUBLIC_INCLUDE}/slt/debug/validated.h
  ${SLT_TECH_PUBLIC_INCLUDE}/slt/debug/details/validated_impl.h
)

add_library(slt_debug OBJECT
  ${SLT_DEBUG_PRIVATE}
  ${SLT_DEBUG_PUBLIC}
)

SLT_MODULE_CONFIG(slt_debug)

slt_unit_test(assert_test debug)
slt_unit_test(invariant_test debug)
slt_unit_test(validated_test debug)