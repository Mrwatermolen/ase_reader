cmake_minimum_required(VERSION 3.20)

project(ASE_READER VERSION 0.0.0 LANGUAGES CXX)

set(ASE_READER_MAIN_PROJECT OFF)
if(CMAKE_SOURCE_DIR STREQUAL PROJECT_SOURCE_DIR)
  set(ASE_READER_MAIN_PROJECT ON)
endif()

if(PROJECT_SOURCE_DIR STREQUAL PROJECT_BINARY_DIR)
    message(FATAL_ERROR "In-source builds are not allowed")
endif()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

set(ASE_READER_PUBLIC_INCLUDE_DIR include)

add_library(ase_reader INTERFACE)
target_include_directories(ase_reader INTERFACE
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/${ASE_READER_PUBLIC_INCLUDE_DIR}>
  $<INSTALL_INTERFACE:${ASE_READER_PUBLIC_INCLUDE_DIR}>
)

add_subdirectory(src)

set(ASE_READER_CMAKE_DIR ${CMAKE_CURRENT_BINARY_DIR}/cmake)
install(TARGETS ase_reader EXPORT ase_reader-targets)

include(CMakePackageConfigHelpers)

configure_package_config_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/cmake/ase_reader-config.cmake.in
  ${ASE_READER_CMAKE_DIR}/ase_reader-config.cmake
  INSTALL_DESTINATION ${ASE_READER_CMAKE_DIR}
)

export(EXPORT ase_reader-targets
  FILE ${ASE_READER_CMAKE_DIR}/ase_reader-targets.cmake
  NAMESPACE ase_reader::
)
