file(GLOB mex_files ${CMAKE_CURRENT_SOURCE_DIR}/mex/*.c)

foreach(mex_file ${mex_files})
  get_filename_component(mex_name ${mex_file} NAME_WE)
  matlab_add_mex(
    NAME ${mex_name}
    MODULE
    SRC ${mex_file}
    LINK_TO mxIgraph R2018a)
endforeach()
