cmake_minimum_required(VERSION 3.0)
project(levtree)

if(CMAKE_COMPILER_IS_GNUCXX)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -Wpedantic -ansi -std=c11 -march=native")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic -ansi -std=c++14 -march=native")
endif()

if(NOT PYTHON_VERSION)
    set(PYTHON_VERSION 3 CACHE INT "Python version for which generate bindings")
endif()

add_subdirectory(src)

enable_testing()

add_subdirectory(test)

