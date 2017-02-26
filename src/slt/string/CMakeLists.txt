SET(SLT_STRING_PRIVATE
  split.cpp
)

SET(SLT_STRING_PUBLIC
  ${SLT_TECH_PUBLIC_INCLUDE_DIR}/slt/string/join.h
  ${SLT_TECH_PUBLIC_INCLUDE_DIR}/slt/string/split.h
  ${SLT_TECH_PUBLIC_INCLUDE_DIR}/slt/string/string_view.h
)

add_library(slt_string OBJECT
  ${SLT_STRING_PRIVATE}
  ${SLT_STRING_PUBLIC}
)

SLT_MODULE_CONFIG(slt_string)

slt_unit_test(join_test string)
slt_unit_test(split_test string)
slt_unit_test(string_view_test string)