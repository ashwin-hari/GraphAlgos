add_library(GraphAlgos find_subgraph.cpp)

find_package(Boost REQUIRED COMPONENTS graph)
target_link_libraries(GraphAlgos PRIVATE Boost::graph)
target_include_directories(GraphAlgos INTERFACE "${CMAKE_CURRENT_LIST_DIR}")