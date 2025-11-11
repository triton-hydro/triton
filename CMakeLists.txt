cmake_minimum_required(VERSION 3.16 FATAL_ERROR) # minimum CMake version to support Kokkos

set(TRITON_EXECUTABLE "triton.exe")

find_package(Python3 COMPONENTS Interpreter REQUIRED)

# put these before project(...)
include(cmake/util.cmake)
include(cmake/machine.cmake)

set_environment()

project(triton CXX)
set(CMAKE_CXX_STANDARD 17) # minimum C++ standard to support Kokkos
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

message(STATUS "CMAKE_CXX_COMPILER = ${CMAKE_CXX_COMPILER}")

# check if MPI is available
if(MACHINE STREQUAL Windows)
  find_package(MPI REQUIRED COMPONENTS CXX)
endif()

# add external packages
add_subdirectory(${TRITON_SOURCE_DIR}/external)

# build triton
add_executable(${TRITON_EXECUTABLE} ./src/main.cpp)
target_link_libraries(${TRITON_EXECUTABLE} PUBLIC kokkos)

if (ENSEMBLE_BUILD)
  target_link_libraries(${TRITON_EXECUTABLE} PUBLIC yaml-cpp)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DENSEMBLE_BUILD")
endif()

if (ENABLE_GDAL)
  target_include_directories(${TRITON_EXECUTABLE} PUBLIC ${GDAL_INCLUDE_DIR})
  target_link_libraries(${TRITON_EXECUTABLE} PUBLIC ${GDAL_LIBRARY})
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DTRITON_GDAL")
endif()

add_build_and_run_scripts()

# add tests
if(${BUILD_TESTS})
  enable_testing()
  add_subdirectory(${TRITON_SOURCE_DIR}/test)
  add_test_script()
endif()

add_clean_script()

message(STATUS "CMAKE_CXX_FLAGS = ${CMAKE_CXX_FLAGS}")
message(STATUS "CMAKE_EXE_LINKER_FLAGS = ${CMAKE_EXE_LINKER_FLAGS}")

