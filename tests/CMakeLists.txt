file(GLOB mex_files ${CMAKE_CURRENT_SOURCE_DIR}/mxIgraph/mex/*.c)
file(GLOB mxIgraph_tests ${CMAKE_CURRENT_SOURCE_DIR}/mxIgraph/*.m)
file(GLOB toolbox_tests ${CMAKE_CURRENT_SOURCE_DIR}/toolbox/*.m)

foreach(mex_file ${mex_files})
  get_filename_component(mex_name ${mex_file} NAME_WE)
  matlab_add_mex(NAME ${mex_name}
    MODULE
    SRC ${mex_file}
    LINK_TO mxIgraph
    R2018a
  )
endforeach()

foreach(test_file ${mxIgraph_tests})
  get_filename_component(test_name ${test_file} NAME_WE)
  matlab_add_unit_test(NAME ${test_name}
    UNITTEST_FILE ${test_file}
    ADDITIONAL_PATH ${CMAKE_CURRENT_SOURCE_DIR}/mxIgraph
  )
endforeach()

foreach(test_file ${toolbox_tests})
  get_filename_component(test_name ${test_file} NAME_WE)
  matlab_add_unit_test(NAME ${test_name}
    UNITTEST_FILE ${test_file}
    ADDITIONAL_PATH ${PROJECT_SOURCE_DIR}/toolbox
  )
endforeach()
