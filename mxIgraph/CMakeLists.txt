function(find_matlab_lib LIB_NAME OUTVAR)
  if(WIN32)
    set(lib_path
      "${Matlab_ROOT_DIR}/extern/lib/${Matlab_ARCH}/microsoft/${LIB_NAME}.lib"
    )
  else()
    set(lib_name ${LIB_NAME})
    cmake_path(APPEND_STRING lib_name ${CMAKE_SHARED_LIBRARY_SUFFIX})

    set(lib_path ${Matlab_ROOT_DIR})
    cmake_path(APPEND lib_path "bin" ${Matlab_ARCH} ${lib_name})
  endif()

  set(${OUTVAR}
    ${lib_path}
    PARENT_SCOPE)
endfunction()

file(GLOB c_files CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/src/*.c)
file(GLOB cxx_files CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)
add_library(mxIgraph OBJECT ${c_files} ${cxx_files})

if(WIN32)
  set(Matlab_MATHRNG_DEF "${CMAKE_CURRENT_BINARY_DIR}/libmwmathrng.def")
  set(Matlab_RNG_LIBRARY "${CMAKE_CURRENT_BINARY_DIR}/libmwmathrng.lib")

  file(WRITE ${Matlab_MATHRNG_DEF}
    "LIBRARY libmwmathrng\nEXPORTS\nmrRandu\nmrRandi\nmrRandn\n")

  add_custom_command(
    OUTPUT ${Matlab_RNG_LIBRARY}
    COMMAND lib.exe /def:${Matlab_MATHRNG_DEF} /machine:x64
            /out:${Matlab_RNG_LIBRARY}
    DEPENDS ${Matlab_MATHRNG_DEF}
    COMMENT "Generating import library for internal MATLAB RNG")

  add_custom_target(generate_mathrng_lib DEPENDS ${Matlab_RNG_LIBRARY})
else()
  find_matlab_lib("libmwmathrng" Matlab_RNG_LIBRARY)
endif()
find_matlab_lib("libut" Matlab_UT_LIBRARY)

target_link_libraries(
  mxIgraph
  PRIVATE ${Matlab_MEX_LIBRARY} ${Matlab_MX_LIBRARY} ${Matlab_RNG_LIBRARY}
  ${Matlab_UT_LIBRARY}
  PUBLIC igraph)

target_include_directories(
  mxIgraph
  PRIVATE ${Matlab_INCLUDE_DIRS} ${CMAKE_CURRENT_SOURCE_DIR}/src
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

if(WIN32)
  add_dependencies(mxIgraph generate_mathrng_lib)
endif()

set_property(TARGET mxIgraph PROPERTY CXX_STANDARD 20)
