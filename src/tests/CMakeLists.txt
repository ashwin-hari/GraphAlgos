add_executable(GraphAlgosTests find_subgraph_test.cpp)
find_package(GTest CONFIG REQUIRED)
target_link_libraries(GraphAlgosTests PRIVATE GTest::gtest GTest::gtest_main GraphAlgos)