cmake_minimum_required(VERSION 3.0)
project(levtree)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_EXTENSIONS OFF)
option(EMSCRIPTEN_BUILD "Enable to build with emscripten" OFF)
if(CMAKE_COMPILER_IS_GNUCXX)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -Wpedantic -march=native")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic -march=native")
elseif(EMSCRIPTEN_BUILD)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -Wpedantic --bind")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic --bind")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -s WASM=1 -s TOTAL_MEMORY=67108864 --preload-file ./cracklib-small")
endif()

add_subdirectory(src)

enable_testing()

add_subdirectory(test)

