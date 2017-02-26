SET(SLT_CORE_PRIVATE
  core.cpp
)

SET(SLT_CORE_PUBLIC
  ${SLT_TECH_PUBLIC_INCLUDE_DIR}/slt/core/core.h
)


add_library(slt_core OBJECT
  ${SLT_CORE_PRIVATE} ${SLT_CORE_PUBLIC}
)


SLT_MODULE_CONFIG(slt_core)

slt_unit_test(core_test core)