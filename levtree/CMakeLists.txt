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

list( APPEND CMAKE_CXX_FLAGS "-std=c++11 ${CMAKE_CXX_FLAGS}")

#add_library (core OBJECT ${SOURCES})
add_library (levtree SHARED ${SOURCES})
add_library (levtree_static STATIC ${SOURCES})
add_library (levtree++ SHARED ${SOURCES_CPP})

add_executable(main2.x main2.cpp ${SOURCES_CPP} ${SOURCES})
add_executable(main.x main.c ${SOURCES})

install(TARGETS levtree LIBRARY DESTINATION /usr/local/lib)
install(TARGETS levtree++ LIBRARY DESTINATION /usr/local/lib)


