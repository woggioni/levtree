cmake_minimum_required(VERSION 3.0)

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

#INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})
add_library (levtree SHARED ${SOURCES})
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

FIND_PACKAGE(SWIG REQUIRED)
INCLUDE(${SWIG_USE_FILE})

if(NOT PYTHON_VERSION)
    set(PYTHON_VERSION 3 CACHE INT "Python version for which generate bindings")
endif()

SET(CMAKE_SWIG_FLAGS "")

function(add_python_bindings py_version)
    set(PYTHONLIBS_FOUND FALSE)
#    set(PYTHON_INCLUDE_DIR "" CACHE STRING "")
    FIND_PACKAGE(PythonLibs ${py_version} REQUIRED)
    INCLUDE_DIRECTORIES(${PYTHON_INCLUDE_PATH} ${CMAKE_CURRENT_SOURCE_DIR})
    SET(CMAKE_SWIG_OUTDIR ${CMAKE_CURRENT_BINARY_DIR}/swig/python${py_version}/levtree)
    SET_SOURCE_FILES_PROPERTIES(levtree++/levtree.i PROPERTIES CPLUSPLUS ON)
    #SET_SOURCE_FILES_PROPERTIES(levtree.i PROPERTIES SWIG_FLAGS "-includeall")
    SWIG_ADD_MODULE(pylevtree${py_version} python levtree++/levtree.i)
    SWIG_LINK_LIBRARIES(pylevtree${py_version} levtree++ ${PYTHON_LIBRARIES})
    set_target_properties(${SWIG_MODULE_pylevtree${py_version}_REAL_NAME} PROPERTIES
        LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SWIG_OUTDIR}
        OUTPUT_NAME _levtree
        )
    configure_file(__init__.py ${CMAKE_SWIG_OUTDIR}/__init__.py)
    configure_file(setup.py ${CMAKE_CURRENT_BINARY_DIR}/setup.py)
endfunction()

add_python_bindings(${PYTHON_VERSION})
#add_python_bindings(3)

SET(CMAKE_SWIG_OUTDIR ${CMAKE_CURRENT_BINARY_DIR}/swig/java/)
INCLUDE_DIRECTORIES(/usr/lib/jvm/default/include /usr/lib/jvm/default/include/linux)
SWIG_ADD_MODULE(jlevtree java levtree++/levtree.i)
SWIG_LINK_LIBRARIES(jlevtree levtree++)
set_target_properties(${SWIG_MODULE_jlevtree_REAL_NAME} PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SWIG_OUTDIR}
    )

#set_property(TARGET levtree++ PROPERTY OUTPUT_NAME $<TARGET_FILE_NAME:levtree++>)
#get_target_property(LEVTREE_LIBS levtree++ OUTPUT_NAME)
#get_target_property(TEST_VAR levtree++ INTERFACE_LINK_LIBRARIES)

#message("dfdsg ${swig_generated_file_fullname}")

