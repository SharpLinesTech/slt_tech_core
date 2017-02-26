SET(SLT_LOG_PRIVATE
  log.cpp
  log_internal.h
)

SET(SLT_LOG_PUBLIC
  ${SLT_TECH_PUBLIC_INCLUDE_DIR}/slt/log/log.h
)


add_library(slt_log OBJECT
  ${SLT_LOG_PRIVATE} ${SLT_LOG_PUBLIC}
)


SLT_MODULE_CONFIG(slt_log)