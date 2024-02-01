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

set(IGRAPH_USE_INTERNAL_BLAS ON)
set(IGRAPH_USE_INTERNAL_LAPACK ON)
set(IGRAPH_USE_INTERNAL_ARPACK ON)
set(IGRAPH_USE_INTERNAL_GMP ON)
set(IGRAPH_USE_INTERNAL_GLPK ON)
set(IGRAPH_USE_INTERNAL_plfit ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
add_subdirectory(vendor/igraph)

set(CMAKE_INSTALL_RPATH "$ORIGIN/../../lib")
add_subdirectory(mxIgraph)
add_subdirectory(mex)

add_subdirectory(tests)

install(DIRECTORY toolbox DESTINATION matlab-igraph)
install(TARGETS mxIgraph DESTINATION matlab-igraph/toolbox/lib)

file(GLOB mex_files mex/*.c)
foreach(mex_file ${mex_files})
  get_filename_component(mex_name ${mex_file} NAME_WE)
  install(TARGETS ${mex_name} DESTINATION matlab-igraph/toolbox/+igraph/private)
endforeach()
