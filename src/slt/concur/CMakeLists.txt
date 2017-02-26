SET(SLT_CONCUR_PRIVATE
  event_queue.cpp
)

SET(SLT_CONCUR_PUBLIC
  ${SLT_TECH_PUBLIC_INCLUDE}/slt/concur/blocking_queue.h
  ${SLT_TECH_PUBLIC_INCLUDE}/slt/concur/event_queue.h
  ${SLT_TECH_PUBLIC_INCLUDE}/slt/concur/details/blocking_queue_impl.h
  ${SLT_TECH_PUBLIC_INCLUDE}/slt/concur/details/event_queue_impl.h
)

add_library(slt_concur OBJECT
  ${SLT_CONCUR_PRIVATE}
  ${SLT_CONCUR_PUBLIC}
)

SLT_MODULE_CONFIG(slt_concur)

slt_unit_test(blocking_queue_test concur)
slt_unit_test(event_queue_test concur)
