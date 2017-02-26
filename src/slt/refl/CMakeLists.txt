SET(SLT_REFL_PRIVATE
  type.cpp 
  type_lib.cpp 
)

SET(SLT_REFL_PUBLIC
  ${SLT_TECH_PUBLIC_INCLUDE}/slt/refl/base_type.h
  ${SLT_TECH_PUBLIC_INCLUDE}/slt/refl/collection_type.h
  ${SLT_TECH_PUBLIC_INCLUDE}/slt/refl/enum_type.h
  ${SLT_TECH_PUBLIC_INCLUDE}/slt/refl/refl.h
  ${SLT_TECH_PUBLIC_INCLUDE}/slt/refl/string_type.h
  ${SLT_TECH_PUBLIC_INCLUDE}/slt/refl/struct_type.h
  ${SLT_TECH_PUBLIC_INCLUDE}/slt/refl/traits.h
  ${SLT_TECH_PUBLIC_INCLUDE}/slt/refl/type.h
  ${SLT_TECH_PUBLIC_INCLUDE}/slt/refl/type_lib.h
)

add_library(slt_refl OBJECT
  ${SLT_REFL_PRIVATE}
  ${SLT_REFL_PUBLIC}
)

SLT_MODULE_CONFIG(slt_refl)

slt_unit_test(serialize_test refl)
slt_unit_test(typelib_test refl)

