cmake_minimum_required(VERSION 3.0)

FILE(COPY ${CMAKE_CURRENT_SOURCE_DIR}/dictionary.txt DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
set(DICTIONARY_FILE ${CMAKE_CURRENT_BINARY_DIR}/dictionary.txt)

if(EMSCRIPTEN_BUILD)
    set(DICTIONARY_FILE dictionary.txt)
    set(CMAKE_EXECUTABLE_SUFFIX ".html")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} --preload-file '${CMAKE_CURRENT_BINARY_DIR}/dictionary.txt@${DICTIONARY_FILE}'")
else()
    set(DICTIONARY_FILE ${CMAKE_CURRENT_BINARY_DIR}/dictionary.txt)
endif()
add_executable(cpptest test.cpp)
add_executable(ctest test.c)

target_compile_definitions(cpptest PRIVATE -DDICTIONARY_FILE="${DICTIONARY_FILE}")
target_compile_definitions(ctest PRIVATE -DDICTIONARY_FILE="${DICTIONARY_FILE}")

add_test(cxx_test cpptest)
add_test(c_test ctest)

target_link_libraries(ctest levtree)
target_link_libraries(cpptest levtree++)

if(NOT EMSCRIPTEN_BUILD)
    foreach(PY_VERSION IN ITEMS 2 3)
        if(PYTHON${PY_VERSION}_LIB)
            add_test(NAME python${PY_VERSION}_bindings COMMAND ${PYTHON${PY_VERSION}_EXE} ${CMAKE_SOURCE_DIR}/test/test.py WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
            set_tests_properties(python${PY_VERSION}_bindings PROPERTIES ENVIRONMENT "PYTHONPATH=${CMAKE_BINARY_DIR}/src/swig/python${PY_VERSION}")
        endif()
    endforeach()
endif()

add_executable(utf8_test utf8_test.c)
target_link_libraries(utf8_test levtree)
