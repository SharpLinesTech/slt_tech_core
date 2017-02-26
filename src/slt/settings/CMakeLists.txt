SET(SLT_SETTINGS_PRIVATE
  settings_internal.h
  settings.cpp
)

SET(SLT_SETTINGS_PUBLIC
  ${SLT_TECH_PUBLIC_INCLUDE}/slt/settings/settings.h
  ${SLT_TECH_PUBLIC_INCLUDE}/slt/settings/details/setting_details.h
  ${SLT_TECH_PUBLIC_INCLUDE}/slt/settings/details/setting_impl.h
)

add_library(slt_settings OBJECT
  ${SLT_SETTINGS_PRIVATE} ${SLT_SETTINGS_PUBLIC}
)

SLT_MODULE_CONFIG(slt_settings)

slt_unit_test(settings_test settings)