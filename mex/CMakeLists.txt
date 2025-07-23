configure_file(${CMAKE_CURRENT_SOURCE_DIR}/mexIgraphVersion.h.in
  ${CMAKE_CURRENT_BINARY_DIR}/mexIgraphVersion.h)

file(GLOB src_files CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/private/*.c)
matlab_add_mex(
  NAME mexIgraphDispatcher
  MODULE
  SRC ${src_files} mexIgraphDispatcher.c NO_IMPLICIT_LINK_TO_MATLAB_LIBRARIES
  LINK_TO mxIgraph ${Matlab_MEX_LIBRARY} ${Matlab_MX_LIBRARY} R2018a)

target_include_directories(
  mexIgraphDispatcher PRIVATE ${Matlab_INCLUDE_DIRS}
  ${CMAKE_CURRENT_SOURCE_DIR})

set(top_level_functions "mexIgraphVersion" "mexIgraphIsDirected")
foreach(func ${top_level_functions})
  matlab_add_mex(
    NAME ${func}
    MODULE
    SRC ${func}.c NO_IMPLICIT_LINK_TO_MATLAB_LIBRARIES
    LINK_TO mxIgraph ${Matlab_MEX_LIBRARY} ${Matlab_MX_LIBRARY} R2018a)

  target_include_directories(
    ${func} PRIVATE ${Matlab_INCLUDE_DIRS} ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_BINARY_DIR})
endforeach()
