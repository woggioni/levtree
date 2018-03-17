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

create_library(levtree++ STATIC SHARED ALL INSTALL
    SOURCES ${SOURCES_CPP}
    PUBLIC_INCLUDES $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}> $<INSTALL_INTERFACE:include>
    PUBLIC_LIBS levtree
    PROPERTIES OUTPUT_NAME levtree++ LINKER_LANGUAGE CXX
)

#install(TARGETS levtree++_shared EXPORT levtree++Config
#    ARCHIVE  DESTINATION lib
#    LIBRARY  DESTINATION lib
#    RUNTIME  DESTINATION bin)  # This is for Windows
#install(EXPORT levtree++Config DESTINATION share/levtree/cmake)
#export(TARGETS levtree++_shared FILE levtree++Config.cmake)


file(GLOB_RECURSE HEADERS
  "*.h" "*.hpp"
)
install(FILES ${HEADERS} DESTINATION include/levtree++)

