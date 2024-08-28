configure_file(${CMAKE_CURRENT_SOURCE_DIR}/mexIgraphVersion.h.in
               ${CMAKE_CURRENT_BINARY_DIR}/mexIgraphVersion.h)

file(GLOB src_files CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/private/*.c)
matlab_add_mex(
  NAME mexIgraphDispatcher
  MODULE
  SRC ${src_files} mexIgraphDispatcher.c
  LINK_TO mxIgraph R2018a)

target_include_directories(
  mexIgraphDispatcher PRIVATE ${Matlab_INCLUDE_DIRS}
                              ${CMAKE_CURRENT_SOURCE_DIR})

set(top_level_functions "mexIgraphVersion" "mexIgraphIsDirected")
foreach(func ${top_level_functions})
  matlab_add_mex(
    NAME ${func}
    MODULE
    SRC ${func}.c
    LINK_TO mxIgraph R2018a)

  target_include_directories(
    ${func} PRIVATE ${Matlab_INCLUDE_DIRS} ${CMAKE_CURRENT_SOURCE_DIR}
                    ${CMAKE_CURRENT_BINARY_DIR})
endforeach()
