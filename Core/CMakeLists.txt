# CMake configuration for Core static library
cmake_minimum_required(VERSION 3.10)
project(Core)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB CORE_SRC
    "corners.cpp"
    "cube.cpp"
    "edges.cpp"
    "math.cpp"
    "std2.cpp"
    "tables.cpp"
    "twist.cpp"
    "two_phase.cpp"
)

add_library(core STATIC ${CORE_SRC})

target_include_directories(core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_compile_options(core PRIVATE -mbmi2)
