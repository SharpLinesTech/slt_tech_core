cmake_minimum_required(VERSION 3.1)

project(slt_tech_core)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

set(CMAKE_CONFIGURATION_TYPES Validate;Debug;Release CACHE TYPE INTERNAL FORCE)

set(CMAKE_CXX_FLAGS_VALIDATE ${CMAKE_CXX_FLAGS_DEBUG})
set(CMAKE_C_FLAGS_VALIDATE ${CMAKE_C_FLAGS_DEBUG})
set(CMAKE_C_FLAGS_VALIDATE ${CMAKE_C_FLAGS_DEBUG})
set(CMAKE_EXE_LINKER_FLAGS_VALIDATE ${CMAKE_EXE_LINKER_FLAGS_DEBUG})

if (MSVC)
  add_definitions(-DWIN32_LEAN_AND_MEAN)
  foreach (flag_var
               CMAKE_CXX_FLAGS 
               CMAKE_CXX_FLAGS_DEBUG 
               CMAKE_CXX_FLAGS_RELEASE
               CMAKE_CXX_FLAGS_VALIDATED
               CMAKE_C_FLAGS 
               CMAKE_C_FLAGS_DEBUG 
               CMAKE_C_FLAGS_RELEASE
               CMAKE_C_FLAGS_MINSIZEREL
               CMAKE_C_FLAGS_VALIDATE)
    string(REPLACE "/MD" "-MT" ${flag_var} "${${flag_var}}")
  endforeach()

  SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd4521")
endif()

set(CMAKE_CXX_FLAGS_VALIDATE "${CMAKE_CXX_FLAGS_VALIDATE} -DSLT_ENABLE_INVARIANT_TESTING")
set(CMAKE_C_FLAGS_VALIDATE "${CMAKE_C_FLAGS_VALIDATE} -DSLT_ENABLE_INVARIANT_TESTING")

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DSLT_ENABLE_ASSERTS")
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DSLT_ENABLE_ASSERTS")

set(SLT_TECH_PUBLIC_INCLUDE
  ${CMAKE_CURRENT_SOURCE_DIR}/include
)

set(SLT_TECH_INCLUDE 
  ${SLT_TECH_PUBLIC_INCLUDE}
  ${CMAKE_CURRENT_SOURCE_DIR}/src
  ${CMAKE_CURRENT_SOURCE_DIR}/third_party/glm
  ${CMAKE_CURRENT_SOURCE_DIR}/third_party/spdlog/include)

include_directories(${SLT_TECH_INCLUDE})

# Build thirdparties
add_subdirectory(third_party/googletest EXCLUDE_FROM_ALL)

SET(THIRD_PARTY_TARGETS
  gtest gmock gtest_main gmock_main
)

foreach(TPT ${THIRD_PARTY_TARGETS})
  set_target_properties(${TPT} PROPERTIES FOLDER "third_party")
  set_target_properties(${TPT} PROPERTIES DEBUG_POSTFIX _d)
  set_target_properties(${TPT} PROPERTIES VALIDATE_POSTFIX _v)
endforeach()

enable_testing()

function(slt_unit_test test_name module_name)
  include_directories(${gtest_SOURCE_DIR}/include)
  add_executable(${test_name} "${test_name}.cpp" )
  target_link_libraries(${test_name} slt_tech_core gtest_main)
  add_test(${test_name} ${test_name})
  set_target_properties(${test_name} PROPERTIES FOLDER "tests/${module_name}")
endfunction()

macro(SLT_MODULE_CONFIG tgt)
  set_target_properties(${tgt} PROPERTIES FOLDER "modules")
endmacro()

add_library(slt_tech_core
  $<TARGET_OBJECTS:slt_concur>
  $<TARGET_OBJECTS:slt_core>
  $<TARGET_OBJECTS:slt_debug>
  $<TARGET_OBJECTS:slt_file>
  $<TARGET_OBJECTS:slt_log>
  $<TARGET_OBJECTS:slt_refl>
  $<TARGET_OBJECTS:slt_settings>
  $<TARGET_OBJECTS:slt_string>
)

set_target_properties(slt_tech_core PROPERTIES FOLDER "libs")

add_subdirectory(src/slt/concur)
add_subdirectory(src/slt/core)
add_subdirectory(src/slt/cpp)
add_subdirectory(src/slt/debug)
add_subdirectory(src/slt/file)
add_subdirectory(src/slt/log)
add_subdirectory(src/slt/math)
add_subdirectory(src/slt/mem)
add_subdirectory(src/slt/refl)
add_subdirectory(src/slt/settings)
add_subdirectory(src/slt/string)
add_subdirectory(src/slt/units)

install(TARGETS slt_tech_core
  ARCHIVE DESTINATION lib/$<CONFIG>
  RUNTIME DESTINATION bin/$<CONFIG>
  PUBLIC_HEADER DESTINATION include
)

# Install public headers
install(DIRECTORY include/slt/
  DESTINATION include/slt
  FILES_MATCHING PATTERN "*.h"
)

# Install public headers
install(DIRECTORY third_party/spdlog/include/
  DESTINATION include/
)

install(DIRECTORY third_party/glm/glm/
  DESTINATION include/glm/
)