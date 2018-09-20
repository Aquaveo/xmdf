set(CMAKE_CXX_STANDARD 11)
cmake_minimum_required(VERSION 3.1.2)
cmake_policy(SET CMP0015 NEW) # Link Directory Pathing
set(CMAKE_DEBUG_POSTFIX _d)

if (APPLE)  
  set(CMAKE_POSITION_INDEPENDENT_CODE False)
else()
  set(CMAKE_POSITION_INDEPENDENT_CODE True)
endif()

set(BUILD_TESTING NO CACHE BOOL "Enable/Disable testing")
set(IS_CONDA_BUILD NO CACHE BOOL "Set this if you want to make a conda package.")
set(PYTHON_TARGET_VERSION 3.6 CACHE STRING "Version of python to link to for python wrapping.")
set(CONDA_PREFIX "" CACHE PATH "Path to the conda environment used to build.")
set(IS_PYTHON_BUILD NO CACHE BOOL "Set this if you want to build the python bindings.")
set(XMSSTAMPER_TEST_PATH "../test_files/" CACHE PATH "Path to test files for testing")

project(xmdf C CXX)

if(WIN32)
    if(XMS_BUILD)
        add_definitions(/D _WIN32_WINNT=0x0501)  # Windows XP and higher
        add_definitions(/Zc:wchar_t-)  # Treat wchar_t as built-in type
    else(NOT XMS_BUILD)
        add_definitions(/D BOOST_ALL_NO_LIB)
    endif()
endif()

if(IS_CONDA_BUILD)
  include(${CMAKE_CURRENT_LIST_DIR}/condabuildinfo.cmake)
else() # If we are not using conda, we are using conan
  # Conan setup
  include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
  conan_basic_setup(TARGETS)
  set(EXT_INCLUDE_DIRS ${CONAN_INCLUDE_DIRS})
  set(EXT_LIB_DIRS ${CONAN_LIB_DIRS})
  set(EXT_LIBS ${CONAN_LIBS})
endif(IS_CONDA_BUILD)

message("External Include Dirs: ${EXT_INCLUDE_DIRS}")
message("External Lib Dirs: ${EXT_LIB_DIRS}")
message("Extneral Libs: ${EXT_LIBS}")

include_directories(${CMAKE_CURRENT_LIST_DIR})
include_directories(${EXT_INCLUDE_DIRS})
link_directories(${EXT_LIB_DIRS})

# Sources
set(xmdf_sources
	xmdf/xmdf/XmXmdf.cpp
	xmdf/xmdf/XmXmdfIo.cpp
	xmdf/xmdf/TutStamping.cpp
	xmdf/xmdf/detail/XmBathymetryIntersector.cpp
	xmdf/xmdf/detail/XmBreaklines.cpp
	xmdf/xmdf/detail/XmGuideBankUtil.cpp
	xmdf/xmdf/detail/XmSlopedAbutmentUtil.cpp
	xmdf/xmdf/detail/XmStampEndCap.cpp
	xmdf/xmdf/detail/XmStampInterpCrossSection.cpp
	xmdf/xmdf/detail/XmStampTests.cpp
	xmdf/xmdf/detail/XmUtil.cpp
)

set(xmdf_headers
  xmdf/xmdf/XmXmdf.h
  xmdf/xmdf/XmXmdfIo.h
  xmdf/xmdf/TutStamping.t.h
  xmdf/xmdf/detail/XmBathymetryIntersector.h
  xmdf/xmdf/detail/XmBathymetryIntersector.t.h
  xmdf/xmdf/detail/XmBreaklines.h
  xmdf/xmdf/detail/XmGuideBankUtil.h
  xmdf/xmdf/detail/XmSlopedAbutmentUtil.h
  xmdf/xmdf/detail/XmStampEndCap.h
  xmdf/xmdf/detail/XmXmdf3dPts.h
  xmdf/xmdf/detail/XmStampInterpCrossSection.h
  xmdf/xmdf/detail/XmStampInterpCrossSection.t.h
  xmdf/xmdf/detail/XmStampTests.t.h
  xmdf/xmdf/detail/XmUtil.h
)

# Pybind11 sources
set(xmdf_py
  xmdf/python/xmdf_py.cpp
  #Xmdf
  xmdf/python/xmdf/xmdf_py.cpp
  xmdf/python/xmdf/XmXmdf_py.cpp
  xmdf/python/xmdf/XmXmdfIo_py.cpp
)

set(xmdf_py_headers
  xmdf/python/xmdf/xmdf_py.h
)

# Tests
if (BUILD_TESTING)
 add_definitions(-DXMSSTAMPER_TEST_PATH="${XMSSTAMPER_TEST_PATH}/")
 add_definitions(-DCXX_TEST -DCXXTEST4)

 list(APPEND xmdf_sources
  xmdf/xmdf/detail/XmUtil.t.h
 )

 find_package(CxxTest)
 if(CXXTEST_FOUND)
   include_directories(${CXXTEST_INCLUDE_DIR})
   enable_testing()

   set(CXXTEST_TESTGEN_ARGS --xunit-printer --have-eh)
   file(GLOB_RECURSE test_headers ${CMAKE_CURRENT_LIST_DIR}/xmdf/*.t.h)
      CXXTEST_ADD_TEST(
        runner runner.cpp ${test_headers}
      )
      target_link_libraries(runner ${PROJECT_NAME})
 endif()
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
    target_link_libraries(${PROJECT_NAME} rt)
endif()

#Pybind11
if(IS_PYTHON_BUILD)
      # linux and mac builds for conan (on TRAVISCI) use a docker that has python
    # 2.7 as system python. We do not have control over that docker image so we
    # can't change this and it is fine for building conan packages and checking
    # that we don't have errors in the python wrapping. We have conda recipes
    # for building python packages that target other versions of python.
    find_package(PythonLibs ${PYTHON_TARGET_VERSION} EXACT REQUIRED)
    
    message("PYTHON_INCLUDE: ${PYTHON_INCLUDE_DIRS}")
    message("PYTHON_LIBS: ${PYTHON_LIBRARIES}")

    # Pybind11 module
    if(IS_CONDA_BUILD)
        include("${CONDA_PREFIX}/share/cmake/pybind11/pybind11Targets.cmake")
        include("${CONDA_PREFIX}/share/cmake/pybind11/FindPythonLibsNew.cmake")
        include("${CONDA_PREFIX}/share/cmake/pybind11/pybind11Config.cmake")
        include("${CONDA_PREFIX}/share/cmake/pybind11/pybind11ConfigVersion.cmake")
        include("${CONDA_PREFIX}/share/cmake/pybind11/pybind11Tools.cmake")

    else()
        include("${CONAN_PYBIND11_ROOT}/share/cmake/pybind11/pybind11Targets.cmake")
        include("${CONAN_PYBIND11_ROOT}/share/cmake/pybind11/FindPythonLibsNew.cmake")
        include("${CONAN_PYBIND11_ROOT}/share/cmake/pybind11/pybind11Config.cmake")
        include("${CONAN_PYBIND11_ROOT}/share/cmake/pybind11/pybind11ConfigVersion.cmake")
        include("${CONAN_PYBIND11_ROOT}/share/cmake/pybind11/pybind11Tools.cmake")
    endif()

    pybind11_add_module(xmdf_py
      ${xmdf_py} ${xmdf_py_headers}
    )
    target_include_directories(xmdf_py
      PRIVATE
          ${EXT_LIBS}
          PYTHON_INCLUDE_DIRS
    )
    target_link_libraries(xmdf_py
    PRIVATE
      ${EXT_LIBS}
      ${PROJECT_NAME}
  )
    set_target_properties(xmdf_py PROPERTIES
    LINKER_LANGUAGE CXX
  )

  execute_process(COMMAND python -c "import os; print('${PYTHON_SITE_PACKAGES}'.replace('${CONDA_PREFIX}' + os.sep, ''))" OUTPUT_VARIABLE PYTHON_SITE_PACKAGES_TRIMMED OUTPUT_STRIP_TRAILING_WHITESPACE)

  # Install recipe
  install(
      TARGETS xmdf_py
      ARCHIVE DESTINATION ${PYTHON_SITE_PACKAGES_TRIMMED}
      LIBRARY DESTINATION ${PYTHON_SITE_PACKAGES_TRIMMED}
  )

endif()


# Install recipe
install(
  TARGETS ${PROJECT_NAME}
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib
)
foreach (header IN LISTS xmdf_headers xmdf_py_headers)
  get_filename_component(subdir "${header}" DIRECTORY)
  install(
    FILES "${header}"
    DESTINATION "include/${subdir}"
  )
endforeach ()
