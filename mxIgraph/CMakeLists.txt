file(GLOB src_files ${CMAKE_CURRENT_SOURCE_DIR}/src/*.c)
add_library(mxIgraph
  OBJECT
  ${src_files}
)

target_link_libraries(mxIgraph
  PRIVATE ${Matlab_MEX_LIBRARY} ${Matlab_MX_LIBRARY}
  PUBLIC igraph
)

target_include_directories(mxIgraph
  PRIVATE ${Matlab_INCLUDE_DIRS} ${CMAKE_CURRENT_SOURCE_DIR}/src
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
)
