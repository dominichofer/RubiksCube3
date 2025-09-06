cmake_minimum_required(VERSION 3.10)
project(RubiksCube3)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Enable BMI2 intrinsics globally
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mbmi2")

add_subdirectory(Core)
add_subdirectory(CoreTest)
add_subdirectory(Solver)
