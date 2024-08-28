file(GLOB mex_files CONFIGURE_DEPENDS
  ${CMAKE_CURRENT_SOURCE_DIR}/mxIgraph/mex/*.c)
file(GLOB mxIgraph_tests CONFIGURE_DEPENDS
  ${CMAKE_CURRENT_SOURCE_DIR}/mxIgraph/*.m)
file(GLOB toolbox_tests CONFIGURE_DEPENDS
  ${CMAKE_CURRENT_SOURCE_DIR}/toolbox/*.m)

foreach(mex_file ${mex_files})
  get_filename_component(mex_name ${mex_file} NAME_WE)
  matlab_add_mex(
    NAME ${mex_name}
    MODULE
    SRC ${mex_file}
    LINK_TO mxIgraph R2018a)
endforeach()

foreach(test_file ${mxIgraph_tests})
  get_filename_component(test_name ${test_file} NAME_WE)
  matlab_add_unit_test(
    NAME
    ${test_name}
    UNITTEST_FILE
    ${test_file}
    CUSTOM_TEST_COMMAND
    "run(runner, suite), exit(max([ans(1, :).Failed]))"
    UNITTEST_PRECOMMAND
    "import matlab.unittest.plugins.XMLPlugin; suite = testsuite('${test_name}'); runner = testrunner('textoutput'); xmlFile = '${CMAKE_CURRENT_BINARY_DIR}/test_${test_name}_results.xml'; p = XMLPlugin.producingJUnitFormat(xmlFile); addPlugin(runner, p);"
    ADDITIONAL_PATH
    ${CMAKE_CURRENT_SOURCE_DIR}/mxIgraph
    ${CMAKE_CURRENT_BINARY_DIR}
    ${PROJECT_SOURCE_DIR}/toolbox)
endforeach()

foreach(test_file ${toolbox_tests})
  get_filename_component(test_name ${test_file} NAME_WE)
  matlab_add_unit_test(
    NAME
    ${test_name}
    UNITTEST_FILE
    ${test_file}
    CUSTOM_TEST_COMMAND
    "run(runner, suite), exit(max([ans(1, :).Failed]))"
    UNITTEST_PRECOMMAND
    "import matlab.unittest.plugins.XMLPlugin; suite = testsuite('${test_name}'); runner = testrunner('textoutput'); xmlFile = '${CMAKE_CURRENT_BINARY_DIR}/test_${test_name}_results.xml'; p = XMLPlugin.producingJUnitFormat(xmlFile); addPlugin(runner, p);"
    ADDITIONAL_PATH
    ${PROJECT_SOURCE_DIR}/toolbox
    ${PROJECT_BINARY_DIR}/mex)
endforeach()
