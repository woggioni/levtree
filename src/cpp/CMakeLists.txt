cmake_minimum_required(VERSION 3.0)
project(LEVTREE++)

set(SOURCES_CPP
        levtree++/Levtree.cpp
)

if(NOT EMSCRIPTEN_BUILD)
    set(LIBTYPE SHARED)
else()
    set(LIBTYPE STATIC)
endif()

add_library (levtree++ ${LIBTYPE} ${SOURCES_CPP})

target_include_directories(levtree++ PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        $<INSTALL_INTERFACE:include>
        )

target_link_libraries(levtree++ PUBLIC levtree)

install(TARGETS levtree++ EXPORT levtree++Config
    ARCHIVE  DESTINATION lib
    LIBRARY  DESTINATION lib
    RUNTIME  DESTINATION bin)  # This is for Windows
install(EXPORT levtree++Config DESTINATION share/levtree/cmake)
export(TARGETS levtree++ FILE levtree++Config.cmake)


file(GLOB_RECURSE HEADERS
  "*.h" "*.hpp"
)
install(FILES ${HEADERS} DESTINATION include/levtree++)

