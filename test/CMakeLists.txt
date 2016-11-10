cmake_minimum_required(VERSION 3.0)

add_executable(test_cpp.x test.cpp)
add_executable(test_c.x test.c)

add_test(cxx_test test_cpp.x)
add_test(c_test test_c.x)

target_link_libraries(test_c.x levtree)
target_link_libraries(test_cpp.x levtree++)

