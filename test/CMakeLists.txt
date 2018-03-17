cmake_minimum_required(VERSION 3.0)

FILE(COPY ${CMAKE_CURRENT_SOURCE_DIR}/dictionary.txt DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

if(EMSCRIPTEN_BUILD)
    set(DICTIONARY_FILE dictionary.txt)
    set(CMAKE_EXECUTABLE_SUFFIX ".html")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -s WASM=1 --llvm-lto 3 -s ALLOW_MEMORY_GROWTH=1")
else()
    set(DICTIONARY_FILE ${CMAKE_CURRENT_BINARY_DIR}/dictionary.txt)
endif()

EMBED(ID EMBEDDED_DICTIONARY_FILE
    DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/embeddded/dictionary
    BLOBS levtree_test_dictionary_file=${CMAKE_CURRENT_SOURCE_DIR}/dictionary.txt
)

create_executable(cpptest ALL
    SOURCES test.cpp ${EMBEDDED_DICTIONARY_FILE_OUTPUT}
    PRIVATE_INCLUDES ${CMAKE_CURRENT_BINARY_DIR}/embeddded
    PRIVATE_LIBS levtree++_static
)

create_executable(ctest ALL
    SOURCES test.c ${EMBEDDED_DICTIONARY_FILE_OUTPUT}
    PRIVATE_INCLUDES ${CMAKE_CURRENT_BINARY_DIR}/embeddded
    PRIVATE_LIBS levtree_static
)

add_test(cxx_test cpptest)
add_test(c_test ctest)


if(NOT EMSCRIPTEN_BUILD)
    foreach(PY_VERSION IN ITEMS 2 3)
        if(PYTHON${PY_VERSION}_LIB)
            add_test(NAME python${PY_VERSION}_bindings COMMAND ${PYTHON${PY_VERSION}_EXE} ${CMAKE_SOURCE_DIR}/test/test.py WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
            set_tests_properties(python${PY_VERSION}_bindings PROPERTIES ENVIRONMENT "PYTHONPATH=${CMAKE_BINARY_DIR}/src/swig/python${PY_VERSION}")
        endif()
    endforeach()
endif()

create_executable(utf8_test ALL
    SOURCES utf8_test.c
    PRIVATE_LIBS levtree_static
)

