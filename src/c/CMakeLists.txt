cmake_minimum_required(VERSION 3.0)
project(LEVTREE)

set(SOURCES
    levtree/levnode.c
    levtree/levtree_standing.c
    levtree/levtree.c
    levtree/common.h
)


create_library(levtree STATIC SHARED ALL INSTALL
    SOURCES ${SOURCES}
    PUBLIC_INCLUDES $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}> $<INSTALL_INTERFACE:include>
    PROPERTIES OUTPUT_NAME levtree LINKER_LANGUAGE C
)

#install(TARGETS levtree_shared EXPORT levtreeConfig
#    ARCHIVE  DESTINATION lib
#    LIBRARY  DESTINATION lib
#    RUNTIME  DESTINATION bin)  # This is for Windows
#install(EXPORT levtreeConfig DESTINATION share/levtree/cmake)
#export(TARGETS levtree_shared FILE levtreeConfig.cmake)

file(GLOB_RECURSE HEADERS
  "*.h" "*.hpp"
)
install(FILES ${HEADERS} DESTINATION include/levtree)


