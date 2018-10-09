set(CMAKE_CXX_STANDARD 11)
cmake_minimum_required(VERSION 3.1.2)
cmake_policy(SET CMP0015 NEW) # Link Directory Pathing
set(CMAKE_DEBUG_POSTFIX _d)
set(CMAKE_POSITION_INDEPENDENT_CODE True)

set(BUILD_TESTING NO CACHE BOOL "Enable/Disable testing")
set(XMDF_TEST_PATH "test_files/" CACHE PATH "Path to test files for testing")

project(xmdf C CXX)

add_definitions(/DXMDF_EXPORTS)  # Export symbols
add_definitions(/D_CRT_SECURE_NO_WARNINGS)  # suppress warnings

if(WIN32)
    if(XMS_BUILD)
        add_definitions(/D _WIN32_WINNT=0x0501)  # Windows XP and higher
        add_definitions(/Zc:wchar_t-)  # Treat wchar_t as built-in type
    else(NOT XMS_BUILD)
        add_definitions(/D BOOST_ALL_NO_LIB)
    endif()
endif()

# Conan setup
include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup(TARGETS)
set(EXT_INCLUDE_DIRS ${CONAN_INCLUDE_DIRS})
set(EXT_LIB_DIRS ${CONAN_LIB_DIRS})
set(EXT_LIBS ${CONAN_LIBS})

message("External Include Dirs: ${EXT_INCLUDE_DIRS}")
message("External Lib Dirs: ${EXT_LIB_DIRS}")
message("External Libs: ${EXT_LIBS}")

include_directories(${CMAKE_CURRENT_LIST_DIR}/..)
include_directories(${EXT_INCLUDE_DIRS})
link_directories(${EXT_LIB_DIRS} ../../xmdf)

# Sources
set(xmdf_sources
  xmdf/xmdf/Xmdf.c
  xmdf/xmdf/Xmdf_f.c
  xmdf/xmdf/xmdf_private.c
  xmdf/xmdf/xmdf_timestep.c
)

set(xmdf_headers
  xmdf/xmdf/xmdf/ErrorDefinitions.h
  xmdf/xmdf/xmdf/Xmdf.h
  xmdf/xmdf/xmdf/Xmdf_f.h
  xmdf/xmdf/xmdf/xmdf_private.h
  xmdf/xmdf/xmdf/xmdf_timestep.h
)

# Tests
if (BUILD_TESTING) # perhaps add CXXTEST that runs the c and Fortran tests and file comparisons
 add_definitions(-DXMDF_TEST_PATH="${XMDF_TEST_PATH}/")
 add_definitions(-DCXX_TEST -DCXXTEST4)

 list(APPEND xmdf_sources
  # xmdf/xmdf/detail/XmUtil.t.h
 )
 
 add_subdirectory(xmdf/test/C/source TestC)

# install(
#  TARGETS TestC
#  DESTINATION bin
# )
endif ()

# Static library
add_library(${PROJECT_NAME} STATIC
  ${xmdf_sources} ${xmdf_headers}
)
target_include_directories(${PROJECT_NAME}
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}>
    $<INSTALL_INTERFACE:include/>
    ${Boost_INCLUDE_DIR}
 )
find_package(Threads REQUIRED)
target_link_libraries(${PROJECT_NAME}
  ${EXT_LIBS}
)
target_link_libraries(${PROJECT_NAME}
  ${CMAKE_THREAD_LIBS_INIT}
)
if(UNIX AND NOT APPLE)
    target_link_libraries(${PROJECT_NAME} rt) # no UNIX used
endif()

# Install recipe
install(
  TARGETS ${PROJECT_NAME}
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib
)
foreach (header IN LISTS xmdf_headers)
  get_filename_component(subdir "${header}" DIRECTORY)
  install(
    FILES "${header}"
    DESTINATION "include/${subdir}"
  )
endforeach ()
