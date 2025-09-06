cmake_minimum_required(VERSION 3.10)
project(RubiksCube3)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Enable BMI2 intrinsics globally
if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mbmi2")
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    # For MSVC, enable AVX2 which includes BMI2 support
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /arch:AVX2")
endif()

add_subdirectory(Core)
add_subdirectory(CoreTest)
add_subdirectory(Solver)
