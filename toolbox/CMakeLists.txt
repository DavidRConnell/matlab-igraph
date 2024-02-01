file(GLOB src_files ${CMAKE_CURRENT_SOURCE_DIR}/mex/*.c)
foreach(source ${src_files})
  get_filename_component(mex_name ${source} NAME_WE)
  matlab_add_mex(NAME ${mex_name}
    MODULE
    SRC ${source}
    LINK_TO mxIgraph
    R2018a
  )

  target_include_directories(${mex_name}
    PRIVATE ${Matlab_INCLUDE_DIRS} ${CMAKE_CURRENT_SOURCE_DIR}/mex
    ${PROJECT_SOURCE_DIR}/mxIgraph/include
  )
endforeach()
