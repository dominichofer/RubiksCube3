# CMake configuration for Solver executable
cmake_minimum_required(VERSION 3.10)
project(Solver)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(solver main.cpp)

target_include_directories(solver PRIVATE ${CMAKE_CURRENT_SOURCE_DIR} ../Core)
target_link_libraries(solver core)
