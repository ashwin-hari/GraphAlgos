
#include "find_subgraph.h"

#include <gtest/gtest.h>

#include <algorithm>
#include <functional>
#include <string_view>
#include <unordered_set>

static void PrintGraph(boost::adjacency_list<> const &graph,
                       std::string_view graphName,
                       std::vector<uint64_t> const &assignments = {}) {
  auto getVertexId = [&assignments](uint64_t v) -> uint64_t {
    if (assignments.empty())
      return v;
    else
      return assignments.at(v);
  };

  std::cout << graphName << ":" << std::endl;

  boost::graph_traits<boost::adjacency_list<>>::vertex_iterator i, end;
  boost::graph_traits<boost::adjacency_list<>>::adjacency_iterator ai, a_end;

  bool isPrintVertex = false;
  for (boost::tie(i, end) = boost::vertices(graph); i != end; ++i) {
    isPrintVertex = true;
    std::cout << getVertexId(*i);
    boost::tie(ai, a_end) = boost::adjacent_vertices(*i, graph);
    std::cout << "'s neighbors: ";

    bool isPrintNeighbors = false;
    for (; ai != a_end; ++ai) {
      isPrintNeighbors = true;
      std::cout << getVertexId(*ai);
      if (boost::next(ai) != a_end)
        std::cout << ", ";
    }

    if (!isPrintNeighbors)
      std::cout << "Empty";
    std::cout << std::endl;
  }

  if (!isPrintVertex)
    std::cout << "Empty";

  std::cout << std::endl << std::endl;
}

TEST(GraphAlgos, FindGraph1D) {
  // 1. make a line graph
  boost::adjacency_list<> line;
  add_edge(0, 1, line);
  add_edge(1, 2, line);
  add_edge(2, 3, line);
  add_edge(3, 4, line);
  add_edge(4, 5, line);

  // 2. make a smaller line
  boost::adjacency_list<> small_line;
  add_edge(0, 1, small_line);
  add_edge(1, 2, small_line);
  add_edge(2, 3, small_line);

  PrintGraph(line, "Line");
  PrintGraph(small_line, "Small line");

  auto subgraph = GraphAlgos::FindSubgraph(line, small_line);
  ASSERT_TRUE(subgraph);
  PrintGraph(small_line, "Subgraph", *subgraph);
  EXPECT_TRUE(subgraph->size() == small_line.m_vertices.size())
      << "Subgraph has "
      << subgraph->size() + " vertices, but expected to have "
      << small_line.m_vertices.size() + " vertices";
  std::unordered_set<uint64_t> subgraphSet(subgraph->begin(), subgraph->end());
  EXPECT_TRUE(subgraphSet.size() == small_line.m_vertices.size())
      << "Subgraph assignments are not unique";
}

TEST(GraphAlgos, FindSquare) {
  // 1. make a square graph with extra diagonal edges
  boost::adjacency_list<> squareWithDiagonals;
  add_edge(0, 1, squareWithDiagonals);
  add_edge(1, 2, squareWithDiagonals);
  add_edge(2, 3, squareWithDiagonals);
  add_edge(3, 0, squareWithDiagonals);
  add_edge(0, 2, squareWithDiagonals);
  add_edge(1, 3, squareWithDiagonals);

  // 2. make a square
  boost::adjacency_list<> square;
  add_edge(0, 1, square);
  add_edge(1, 2, square);
  add_edge(2, 3, square);
  add_edge(3, 0, square);

  PrintGraph(squareWithDiagonals, "Square with diagonals");
  PrintGraph(square, "Square");

  auto subgraph = GraphAlgos::FindSubgraph(squareWithDiagonals, square);
  ASSERT_TRUE(subgraph);
  PrintGraph(square, "Subgraph", *subgraph);
  EXPECT_TRUE(subgraph->size() == square.m_vertices.size())
      << "Subgraph has "
      << subgraph->size() + " vertices, but expected to have "
      << square.m_vertices.size() + " vertices";
  std::unordered_set<uint64_t> subgraphSet(subgraph->begin(), subgraph->end());
  EXPECT_TRUE(subgraphSet.size() == square.m_vertices.size())
      << "Subgraph assignments are not unique";
}

TEST(GraphAlgos, FindVerticalReflection) {
  // 1. make a square graph with extra diagonal edges
  boost::adjacency_list<> original;
  add_edge(0, 1, original);
  add_edge(1, 2, original);
  add_edge(1, 3, original);
  add_edge(2, 3, original);

  // 2. make a vertical reflection
  boost::adjacency_list<> reflection;
  add_edge(3, 2, reflection);
  add_edge(2, 1, reflection);
  add_edge(2, 0, reflection);
  add_edge(1, 0, reflection);

  PrintGraph(original, "Original");
  PrintGraph(reflection, "Reflection");

  auto subgraphAssignments = GraphAlgos::FindSubgraph(original, reflection);
  ASSERT_TRUE(subgraphAssignments);
  PrintGraph(reflection, "Subgraph", *subgraphAssignments);
  EXPECT_TRUE(subgraphAssignments->size() == reflection.m_vertices.size())
      << "Subgraph has "
      << subgraphAssignments->size() + " vertices, but expected to have "
      << reflection.m_vertices.size() + " vertices";
  std::unordered_set<uint64_t> subgraphSet(subgraphAssignments->begin(),
                                           subgraphAssignments->end());
  EXPECT_TRUE(subgraphSet.size() == reflection.m_vertices.size())
      << "Subgraph assignments are not unique";
}

TEST(GraphAlgos, FindEmpty) {
  boost::adjacency_list<> line;
  add_edge(0, 1, line);

  boost::adjacency_list<> empty;

  PrintGraph(line, "Line");
  PrintGraph(empty, "Empty");

  auto subgraph = GraphAlgos::FindSubgraph(line, empty);
  ASSERT_TRUE(subgraph);
  PrintGraph(empty, "Subgraph", *subgraph);
  EXPECT_TRUE(subgraph->size() == empty.m_vertices.size())
      << "Subgraph has "
      << subgraph->size() + " vertices, but expected to have "
      << empty.m_vertices.size() + " vertices";

  subgraph = GraphAlgos::FindSubgraph(empty, empty);
  ASSERT_TRUE(subgraph);
  PrintGraph(empty, "Subgraph", *subgraph);
  EXPECT_TRUE(subgraph->size() == empty.m_vertices.size())
      << "Subgraph has "
      << subgraph->size() + " vertices, but expected to have "
      << empty.m_vertices.size() + " vertices";
}