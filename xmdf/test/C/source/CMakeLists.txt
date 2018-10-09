set (CMAKE_CXX_STANDARD 11)
project(TestC CXX)
cmake_minimum_required(VERSION 3.1.2)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

add_definitions(/D NO_WARN_MBCS_MFC_DEPRECATION)
add_definitions(/D NO_XMDF_API)
include_directories(../include ../../../xmdf)
link_directories(../../../../dev/build_cpp/lib ${CONAN_LIB_DIRS})
SET( CMAKE_EXE_LINKER_FLAGS_DEBUG
  "${CMAKE_EXE_LINKER_FLAGS_DEBUG} /NODEFAULTLIB:LIBCMTD.lib" )
  
set(CompilerFlags
		CMAKE_CXX_FLAGS
		CMAKE_CXX_FLAGS_DEBUG
		CMAKE_CXX_FLAGS_RELEASE
		CMAKE_C_FLAGS
		CMAKE_C_FLAGS_DEBUG
		CMAKE_C_FLAGS_RELEASE
		)

add_executable(TestC
  ../include/StdAfx.h 
  ../include/TestDatasets.h 
  ../include/TestGeomPaths.h 
  ../include/TestGrid.h
  ../include/TestMesh.h
  ../include/TestTimestep.h
  ../include/TestXmdf.h 

  StdAfx.cpp
  testDatasets.cpp 
  TestGeomPaths.cpp
  TestGrid.cpp
  TestMesh.cpp
  TestTimestep.cpp
  TestXmdf.cpp
)
target_link_libraries(TestC xmdf_d.lib ${CONAN_LIBS})
