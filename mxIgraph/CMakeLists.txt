function(find_matlab_lib LIB_NAME OUTVAR)
  set(lib_name ${LIB_NAME})
  cmake_path(APPEND_STRING lib_name
    ${CMAKE_SHARED_LIBRARY_SUFFIX})

  set(lib_path ${Matlab_ROOT_DIR})
  cmake_path(APPEND lib_path
    "bin" ${Matlab_ARCH} ${lib_name})

  set(${OUTVAR} ${lib_path} PARENT_SCOPE)
endfunction()

file(GLOB c_files CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/src/*.c)
file(GLOB cxx_files CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)
add_library(mxIgraph
  OBJECT
  ${c_files} ${cxx_files}
)

find_matlab_lib("libmwmathrng" Matlab_RNG_LIBRARY)
find_matlab_lib("libut" Matlab_UT_LIBRARY)

target_link_libraries(mxIgraph
  PRIVATE
  ${Matlab_MEX_LIBRARY}
  ${Matlab_MX_LIBRARY}
  ${Matlab_RNG_LIBRARY}
  ${Matlab_UT_LIBRARY}
  PUBLIC
  igraph
)

target_include_directories(mxIgraph
  PRIVATE ${Matlab_INCLUDE_DIRS} ${CMAKE_CURRENT_SOURCE_DIR}/src
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
)
