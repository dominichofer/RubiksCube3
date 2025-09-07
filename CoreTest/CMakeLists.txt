add_executable(
    CoreTest
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

target_link_libraries(
    CoreTest
    Core
    GTest::GTest
    GTest::Main
)

include(GoogleTest)
gtest_discover_tests(CoreTest)
