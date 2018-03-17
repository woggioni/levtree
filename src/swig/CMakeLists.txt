function(add_python_bindings py_version)
    PYFIND(VERSIONS ${py_version} SUFFIX ${py_version} REQUIRED LIB)
    SET(CMAKE_SWIG_OUTDIR ${CMAKE_CURRENT_BINARY_DIR}/python${py_version}/levtree)

    set(IFLAGS "")
    get_target_property(IPATH levtree INTERFACE_INCLUDE_DIRECTORIES)
    foreach(PATH ${IPATH})
        list(APPEND IFLAGS -I${PATH})
    endforeach()
    message("PYTHON_INCLUDE_${py_version}: ${PYTHON_INCLUDE_${py_version}}")
    SET(CMAKE_SWIG_FLAGS -builtin -O -I${PYTHON_INCLUDE_${py_version}} -I${LEVTREE_SOURCE_DIR} -I${LEVTREE++_SOURCE_DIR})
    SET_SOURCE_FILES_PROPERTIES(levtree.i PROPERTIES CPLUSPLUS ON)
    swig_add_library(pylevtree${py_version} LANGUAGE python SOURCES levtree.i)
    SWIG_LINK_LIBRARIES(pylevtree${py_version} levtree++_shared ${PYTHON_LIB_${py_version}})
    TARGET_INCLUDE_DIRECTORIES(_pylevtree${py_version} PRIVATE ${PYTHON_INCLUDE_${py_version}} ${CMAKE_CURRENT_SOURCE_DIR})
    set_target_properties(${SWIG_MODULE_pylevtree${py_version}_REAL_NAME} PROPERTIES
        LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SWIG_OUTDIR}
        OUTPUT_NAME _levtreecpp
        INTERPROCEDURAL_OPTIMIZATION ${CMAKE_INTERPROCEDURAL_OPTIMIZATION}
        )
    configure_file(__init__.py ${CMAKE_SWIG_OUTDIR}/__init__.py)
    set(SETUP_INTERP ${PYTHON${py_version}_EXE})
    configure_file(setup.py ${CMAKE_CURRENT_BINARY_DIR}/python${py_version}/setup.py)
endfunction()

if(PYTHON2_BINDINGS)
    FIND_PACKAGE(SWIG REQUIRED)
    INCLUDE(${SWIG_USE_FILE})
    add_python_bindings(2)
endif()
if(PYTHON3_BINDINGS)
    FIND_PACKAGE(SWIG REQUIRED)
    INCLUDE(${SWIG_USE_FILE})
    add_python_bindings(3)
endif()

if(JAVA_BINDINGS)
    FIND_PACKAGE(SWIG REQUIRED)
    INCLUDE(${SWIG_USE_FILE})
    find_package(JNI)
    if(JNI_FOUND)
        SET(CMAKE_SWIG_OUTDIR ${CMAKE_CURRENT_BINARY_DIR}/java/)
        SET(CMAKE_SWIG_FLAGS -O -I${PYTHON${py_version}_INCLUDE} -I${LEVTREE_SOURCE_DIR} -I${LEVTREE++_SOURCE_DIR})
        SET_SOURCE_FILES_PROPERTIES(levtree.i PROPERTIES CPLUSPLUS ON)
        swig_add_library(jlevtree LANGUAGE java SOURCES levtree.i)
        SWIG_LINK_LIBRARIES(jlevtree levtree++_shared)
        target_include_directories(jlevtree PRIVATE ${JNI_INCLUDE_DIRS})
        set_target_properties(${SWIG_MODULE_jlevtree_REAL_NAME} PROPERTIES
            LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SWIG_OUTDIR}
            INTERPROCEDURAL_OPTIMIZATION ${CMAKE_INTERPROCEDURAL_OPTIMIZATION}
        )
    endif()
endif()
