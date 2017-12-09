cmake_minimum_required(VERSION 3.0)
project(LEVTREE)

set(SOURCES
    levtree/levnode.c
    levtree/levtree_standing.c
    levtree/levtree.c
    levtree/common.h
)


if(NOT EMSCRIPTEN_BUILD)
    set(LIBTYPE SHARED)
else()
    set(LIBTYPE STATIC)
endif()

add_library (levtree ${LIBTYPE} ${SOURCES})

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

file(GLOB_RECURSE HEADERS
  "*.h" "*.hpp"
)
install(FILES ${HEADERS} DESTINATION include/levtree)


