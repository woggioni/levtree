cmake_minimum_required(VERSION 3.0)

add_executable(test_cpp.x test.cpp)
add_executable(test_c.x test.c)

add_test(cxx_test test_cpp.x)
add_test(c_test test_c.x)

target_link_libraries(test_c.x levtree)
target_link_libraries(test_cpp.x levtree++)

FIND_PACKAGE(PythonInterp ${py_version} REQUIRED)
set(ENV{PYTHONPATH} ${CMAKE_BINARY_DIR}/src/swig)
message($ENV{PYTHONPATH})
#add_test(NAME python_bindings COMMAND env)
add_test(NAME python_bindings COMMAND ${PYTHON_EXECUTABLE} ${CMAKE_SOURCE_DIR}/test/test.py WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
set_tests_properties(python_bindings PROPERTIES ENVIRONMENT "PYTHONPATH=${CMAKE_BINARY_DIR}/src/swig/python${PYTHON_VERSION}")
