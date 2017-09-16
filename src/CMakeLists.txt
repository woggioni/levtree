cmake_minimum_required(VERSION 3.0)

set(SOURCES
    levtree/levnode.c
    levtree/levtree_standing.c
    levtree/wlevtree.c
    levtree/levtree.c
    levtree/wlevnode.c
    levtree/common.h
)

set(SOURCES_CPP
        levtree++/Levtree.cpp
)

#INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})
if(NOT EMSCRIPTEN_BUILD)
    set(LIBTYPE SHARED)
else()
    set(LIBTYPE STATIC)
endif()

add_library (levtree ${LIBTYPE} ${SOURCES})
add_library (levtree++ ${LIBTYPE} ${SOURCES_CPP})

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
install(FILES ${HEADERS} DESTINATION include/levtree)

function(add_python_bindings py_version)
    set(PYTHONLIBS_FOUND FALSE)
#    set(PYTHON_INCLUDE_DIR "" CACHE STRING "")
    FIND_PACKAGE(PythonLibs ${py_version} REQUIRED)
    SET(CMAKE_SWIG_OUTDIR ${CMAKE_CURRENT_BINARY_DIR}/swig/python${py_version}/levtree)
    SET(CMAKE_SWIG_FLAGS -O -I${PYTHON_INCLUDE_PATH} -I${CMAKE_CURRENT_SOURCE_DIR})
    SET_SOURCE_FILES_PROPERTIES(levtree++/levtree.i PROPERTIES CPLUSPLUS ON)
    swig_add_library(pylevtree${py_version} LANGUAGE python SOURCES levtree++/levtree.i)
    SWIG_LINK_LIBRARIES(pylevtree${py_version} levtree++ ${PYTHON_LIBRARIES})
    TARGET_INCLUDE_DIRECTORIES(_pylevtree${py_version} PRIVATE ${PYTHON_INCLUDE_PATH} ${CMAKE_CURRENT_SOURCE_DIR})
    set_target_properties(${SWIG_MODULE_pylevtree${py_version}_REAL_NAME} PROPERTIES
        LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SWIG_OUTDIR}
        OUTPUT_NAME _levtree
        )
    configure_file(__init__.py ${CMAKE_SWIG_OUTDIR}/__init__.py)
    configure_file(setup.py ${CMAKE_CURRENT_BINARY_DIR}/setup.py)
endfunction()

if(NOT EMSCRIPTEN_BUILD)
    FIND_PACKAGE(SWIG REQUIRED)
    INCLUDE(${SWIG_USE_FILE})
    add_python_bindings(${PYTHON_VERSION})
    #add_python_bindings(3)

    find_package(JNI)
    if(JNI_FOUND)
        SET(CMAKE_SWIG_OUTDIR ${CMAKE_CURRENT_BINARY_DIR}/swig/java/)
        SET(CMAKE_SWIG_FLAGS -O -I${CMAKE_CURRENT_SOURCE_DIR})
        swig_add_library(jlevtree LANGUAGE java SOURCES levtree++/levtree.i)
        SWIG_LINK_LIBRARIES(jlevtree levtree++)
        target_include_directories(jlevtree PRIVATE ${JNI_INCLUDE_DIRS})
        set_target_properties(${SWIG_MODULE_jlevtree_REAL_NAME} PROPERTIES
            LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SWIG_OUTDIR}
        )
    endif()
endif()
#set_property(TARGET levtree++ PROPERTY OUTPUT_NAME $<TARGET_FILE_NAME:levtree++>)
#get_target_property(LEVTREE_LIBS levtree++ OUTPUT_NAME)
#get_target_property(TEST_VAR levtree++ INTERFACE_LINK_LIBRARIES)

#message("dfdsg ${swig_generated_file_fullname}")

