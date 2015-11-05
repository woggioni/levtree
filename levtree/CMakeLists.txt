cmake_minimum_required(VERSION 3.0)
project(Levtree)

set(CMAKE_BUILD_TYPE Release)
#set(CMAKE_C_COMPILER g++)

set(SOURCES
    levnode.c
    levtree_standing.c 
    wlevtree.c
    levtree.c
    wlevnode.c
)

set(SOURCES_CPP
	cpp/Levtree.cpp
)

enable_testing()


list( APPEND CMAKE_CXX_FLAGS "-std=c++11 ${CMAKE_CXX_FLAGS}")

#add_library (core OBJECT ${SOURCES})
add_library (levtree SHARED ${SOURCES})
#add_library (levtree_static STATIC ${SOURCES})
add_library (levtree++ SHARED ${SOURCES_CPP})

add_executable(test_cpp.x test.cpp)
add_executable(test_c.x test.c)

add_test(cxx_test test_cpp.x)
add_test(c_test test_c.x)

target_link_libraries(test_c.x levtree)
target_link_libraries(test_cpp.x levtree++ levtree)



install(TARGETS levtree LIBRARY DESTINATION /usr/local/lib)
install(TARGETS levtree++ LIBRARY DESTINATION /usr/local/lib)
install(FILES common.h  levnode.h  levtree.h	levtree_standing.h  wlevnode.h	wlevtree.h DESTINATION /usr/local/include/levtree)
install(FILES cpp/Levtree.hpp DESTINATION /usr/local/include/levtree/cpp)

