add_library(
    Core STATIC
    "corners.cpp"
    "cube.cpp"
    "edges.cpp"
    "math.cpp"
    "std2.cpp"
    "tables.cpp"
    "twist.cpp"
    "two_phase.cpp"
)

target_include_directories(Core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(Core PUBLIC OpenMP::OpenMP_CXX)
