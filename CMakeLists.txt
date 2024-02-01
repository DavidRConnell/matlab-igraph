cmake_minimum_required(VERSION 3.18)

project(
  matlab-igraph
  DESCRIPTION "A MATLAB toolbox for using the igraph library"
  LANGUAGES C CXX
)

enable_testing()

find_package(Matlab COMPONENTS MAIN_PROGRAM MAT_LIBRARY MX_LIBRARY MEX_COMPILER REQUIRED)

message(STATUS "Matlab: ${Matlab_VERSION_STRING}")

if(Matlab_VERSION_STRING VERSION_LESS 9.14)
  message(WARNING "Matlab >= 9.14 (R2023a) is required for MEX tests to work due to new buildtool argument syntax.")
endif()

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(BUILD_SHARED_LIBS OFF)

set(IGRAPH_USE_INTERNAL_BLAS ON)
set(IGRAPH_USE_INTERNAL_LAPACK ON)
set(IGRAPH_USE_INTERNAL_ARPACK ON)
set(IGRAPH_USE_INTERNAL_GMP ON)
set(IGRAPH_USE_INTERNAL_GLPK ON)
set(IGRAPH_USE_INTERNAL_plfit ON)
add_subdirectory(vendor/igraph)

include(vendor/igraph/etc/cmake/GetGitRevisionDescription.cmake)
git_describe(CMAKE_PACKAGE_VERSION)
if ("${CMAKE_PACKAGE_VERSION}" STREQUAL "")
  set(CMAKE_PACKAGE_VERSION "NOTFOUND")
endif()

message(STATUS "${CMAKE_PROJECT_NAME} version: ${CMAKE_PACKAGE_VERSION}")
message(STATUS "MEX extension: ${Matlab_MEX_EXTENSION}")
message(STATUS "")

set(CMAKE_INSTALL_RPATH "$ORIGIN/../../lib")
add_subdirectory(mxIgraph)
add_subdirectory(mex)

add_subdirectory(tests)

# Installation
install(DIRECTORY toolbox
  DESTINATION matlab-igraph
  COMPONENT toolbox)

file(GLOB mex_files mex/*.c)
foreach(mex_file ${mex_files})
  get_filename_component(mex_name ${mex_file} NAME_WE)
  install(TARGETS ${mex_name}
    DESTINATION matlab-igraph/toolbox/+igraph/private
    COMPONENT toolbox)
endforeach()

# Packaging
set(CPACK_PACKAGE_HOMEPAGE_URL("https://github.com/DavidRConnell/matlab-igraph"))

set(CPACK_SOURCE_PACKAGE_FILE_NAME "${CMAKE_PROJECT_NAME}_${CMAKE_PACKAGE_VERSION}_source")
set(CPACK_PACKAGE_FILE_NAME
  "${CMAKE_PROJECT_NAME}_${CMAKE_PACKAGE_VERSION}_${CMAKE_SYSTEM_NAME}_${CMAKE_SYSTEM_PROCESSOR}")
set(CPACK_SOURCE_GENERATOR "TGZ")
set(CPACK_GENERATOR "TGZ")
if(CMAKE_SYSTEM MATCHES Windows)
  set(CPACK_GENERATOR "ZIP")
endif()

set(CPACK_SOURCE_IGNORE_FILES
  "\\\\..*/"
  "\\\\.${Matlab_MEX_EXTENSION}$"
  "${CMAKE_SOURCE_DIR}/build"
  "${CMAKE_SOURCE_DIR}/dist"
  "compile_commands.json")

# Only package files needed to run the toolbox.
set(CPACK_ARCHIVE_COMPONENT_INSTALL ON)
get_cmake_property(CPACK_COMPONENTS_ALL COMPONENTS)
list(REMOVE_ITEM CPACK_COMPONENTS_ALL "Unspecified")
include(CPack)
