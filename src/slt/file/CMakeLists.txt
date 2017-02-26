SET(SLT_FILE_PRIVATE
  file.cpp
  file_internal.h
)

SET(SLT_FILE_PUBLIC
  ${SLT_TECH_PUBLIC_INCLUDE_DIR}/slt/file/read.h
  ${SLT_TECH_PUBLIC_INCLUDE_DIR}/slt/file/write.h
  ${SLT_TECH_PUBLIC_INCLUDE_DIR}/slt/file/details/read_impl.h
)


add_library(slt_file OBJECT
  ${SLT_FILE_PRIVATE} ${SLT_FILE_PUBLIC}
)


SLT_MODULE_CONFIG(slt_file)