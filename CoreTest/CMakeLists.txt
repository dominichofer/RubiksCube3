# CMake configuration for CoreTest executable
cmake_minimum_required(VERSION 3.10)
project(CoreTest)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find Google Test
find_package(GTest REQUIRED)

file(GLOB CORETEST_SRC
    "corners.cpp"
    "cube.cpp"
    "edges.cpp"
    "math.cpp"
    "random.cpp"
    "std2.cpp"
    "tables.cpp"
    "twist.cpp"
    "twister.cpp"
    "pch.cpp"
)

add_executable(coretest ${CORETEST_SRC})

target_include_directories(coretest PRIVATE ${CMAKE_CURRENT_SOURCE_DIR} ../Core)
target_link_libraries(coretest core GTest::GTest GTest::Main)
