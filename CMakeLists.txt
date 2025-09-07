cmake_minimum_required(VERSION 3.10)
project(RubiksCube3)
enable_testing()

set(CMAKE_CXX_STANDARD 23)
if (MSVC)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /EHsc /MP /arch:AVX2")
	set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /Od /MDd")
	set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /Ox /MD /GL")
else()
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -march=native -w -mbmi2")
	set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g")
	set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3 -DNDEBUG")
endif()

include(FetchContent)

# Google Test
FetchContent_Declare(googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG v1.15.2
)
FetchContent_MakeAvailable(googletest)
set(BENCHMARK_ENABLE_GTEST_TESTS OFF)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

# OpenMP
find_package(OpenMP)
link_libraries(OpenMP::OpenMP_CXX)

add_subdirectory(Core)
add_subdirectory(CoreTest)
add_subdirectory(Solver)
