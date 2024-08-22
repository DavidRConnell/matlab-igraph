file(GLOB c_files CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/src/*.c)
file(GLOB cxx_files CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)
add_library(mxIgraph
  OBJECT
  ${c_files} ${cxx_files}
)

set(rng_lib_name "libmwmathrng")
cmake_path(APPEND_STRING rng_lib_name
  ${CMAKE_SHARED_LIBRARY_SUFFIX})

set(Matlab_RNG_LIBRARY ${Matlab_ROOT_DIR})
cmake_path(APPEND Matlab_RNG_LIBRARY
  "bin" ${Matlab_ARCH} ${rng_lib_name})

target_link_libraries(mxIgraph
  PRIVATE
  ${Matlab_MEX_LIBRARY}
  ${Matlab_MX_LIBRARY}
  ${Matlab_RNG_LIBRARY}
  PUBLIC
  igraph
)

target_include_directories(mxIgraph
  PRIVATE ${Matlab_INCLUDE_DIRS} ${CMAKE_CURRENT_SOURCE_DIR}/src
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
)
