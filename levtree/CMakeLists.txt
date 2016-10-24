cmake_minimum_required(VERSION 3.0)
project(Levtree)

#set(CMAKE_C_COMPILER g++)

set(SOURCES
    levtree/levnode.c
    levtree/levtree_standing.c
    levtree/wlevtree.c
    levtree/levtree.c
    levtree/wlevnode.c
)

set(SOURCES_CPP
        levtree++/Levtree.cpp
)

enable_testing()

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})
#add_library (core OBJECT ${SOURCES})
add_library (levtree SHARED ${SOURCES})
#add_library (levtree_static STATIC ${SOURCES})
add_library (levtree++ SHARED ${SOURCES_CPP})

target_include_directories(levtree PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        $<INSTALL_INTERFACE:include>
        )

install(TARGETS levtree EXPORT levtreeConfig
    ARCHIVE  DESTINATION lib
    LIBRARY  DESTINATION lib
    RUNTIME  DESTINATION bin)  # This is for Windows
install(EXPORT levtreeConfig DESTINATION share/levtree/cmake)
export(TARGETS levtree FILE levtreeConfig.cmake)

target_include_directories(levtree++ PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        $<INSTALL_INTERFACE:include>
        )

install(TARGETS levtree++ EXPORT levtree++Config
    ARCHIVE  DESTINATION lib
    LIBRARY  DESTINATION lib
    RUNTIME  DESTINATION bin)  # This is for Windows
install(EXPORT levtree++Config DESTINATION share/levtree/cmake)
export(TARGETS levtree++ FILE levtree++Config.cmake)



add_executable(test_cpp.x test/test.cpp)
add_executable(test_c.x test/test.c)

add_test(cxx_test test_cpp.x)
add_test(c_test test_c.x)

target_link_libraries(test_c.x levtree)
target_link_libraries(test_cpp.x levtree++ levtree)


file(GLOB_RECURSE HEADERS
  "*.h" "*.hpp"
)
install(FILES ${HEADERS} DESTINATION include/levtree)



FIND_PACKAGE(SWIG REQUIRED)
INCLUDE(${SWIG_USE_FILE})

FIND_PACKAGE(PythonLibs)
INCLUDE_DIRECTORIES(${PYTHON_INCLUDE_PATH})

SET(CMAKE_SWIG_FLAGS "")

SET_SOURCE_FILES_PROPERTIES(levtree++/levtree.i PROPERTIES CPLUSPLUS ON)
#SET_SOURCE_FILES_PROPERTIES(levtree.i PROPERTIES SWIG_FLAGS "-includeall")
SWIG_ADD_MODULE(Levtree python levtree++/levtree.i ${SOURCES_CPP})
SWIG_LINK_LIBRARIES(Levtree levtree ${PYTHON_LIBRARIES})
