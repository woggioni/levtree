cmake_minimum_required(VERSION 3.0)
project(levtree)

if(${CMAKE_VERSION} VERSION_GREATER "3.9")
    cmake_policy(VERSION 3.9)
endif()

add_subdirectory(cmake)
SETUP_STANDARD_FLAGS()

set(CMAKE_INSTALL_PREFIX /usr/local CACHE PATH "")
option(EMSCRIPTEN_BUILD "Enable to build with emscripten" OFF)
option(PYTHON2_BINDINGS "Build Python 2 bindings with SWIG" OFF)
option(PYTHON3_BINDINGS "Build Python 3 bindings with SWIG" OFF)
option(JAVA_BINDINGS "Build Java bindings with SWIG" OFF)

#if(EMSCRIPTEN_BUILD)
#    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} --bind")
#    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --bind")
#endif()

add_subdirectory(src)

enable_testing()

add_subdirectory(test)

