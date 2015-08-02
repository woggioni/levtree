cmake_minimum_required(VERSION 3.0)
project(Levtree)

set(CMAKE_BUILD_TYPE Release)

set(SOURCES
    levnode.c
    levtree_standing.c 
    wlevtree.c
    levtree.c
    wlevnode.c
)

#add_library (core OBJECT ${SOURCES})
add_library (levtree SHARED ${SOURCES})

install(TARGETS levtree LIBRARY DESTINATION /usr/local/lib)



