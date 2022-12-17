#include "find_subgraph.h"

#include <algorithm>
#include <cstdint>
#include <iostream>
#include <numeric>
#include <vector>

namespace GraphAlgos {
/**
 * @brief Update PossibleAssignments[x] to {x' | all neighbors of x can be
 * possibly assigned to a neighbor of x'} for all x.
 * @return true if the possible assignments were updated and false otherwise
 */
static bool UpdatePossibleAssignments(
    boost::adjacency_list<> const &graph,
    boost::adjacency_list<> const &subgraph,
    std::vector<std::vector<uint64_t>> &possibleAssignments) {
  bool isUpdate = false;
  for (size_t subgraphVertex = 0u; subgraphVertex < possibleAssignments.size();
       subgraphVertex++) {
    boost::graph_traits<boost::adjacency_list<>>::adjacency_iterator
        subgraphNeighborsBegin,
        subgraphNeighborsEnd, graphNeighborsBegin, graphNeighborsEnd;
    boost::tie(subgraphNeighborsBegin, subgraphNeighborsEnd) =
        boost::adjacent_vertices(subgraphVertex, subgraph);
    if (subgraphNeighborsBegin == subgraphNeighborsEnd) {
      continue;
    }
    auto assignedVertexIt = possibleAssignments.at(subgraphVertex).begin();
    while (assignedVertexIt != possibleAssignments.at(subgraphVertex).end()) {
      boost::tie(graphNeighborsBegin, graphNeighborsEnd) =
          boost::adjacent_vertices(*assignedVertexIt, graph);
      // can all neighbors of subgraphVertex be assigned to a neighbor of
      // assignedVertex?
      bool removeAssignment = false;
      for (auto subgraphNeibhbor = subgraphNeighborsBegin;
           subgraphNeibhbor != subgraphNeighborsEnd; subgraphNeibhbor++) {
        auto &neighborAssignments = possibleAssignments.at(*subgraphNeibhbor);
        auto isNeibhorValid = std::any_of(
            neighborAssignments.begin(), neighborAssignments.end(),
            [&](uint64_t neighborAssignment) -> bool {
              return std::find(graphNeighborsBegin, graphNeighborsEnd,
                               neighborAssignment) != graphNeighborsEnd;
            });
        if (!isNeibhorValid) {
          assignedVertexIt =
              possibleAssignments.at(subgraphVertex).erase(assignedVertexIt);
          removeAssignment = true;
          break;
        }
      }
      isUpdate |= removeAssignment;
      if (!removeAssignment) {
        assignedVertexIt++;
      }
    }
  }
  return isUpdate;
}

static bool DoAssignedEdgesExist(boost::adjacency_list<> const &graph,
                                 boost::adjacency_list<> const &subgraph,
                                 std::vector<uint64_t> const &assignments) {
  boost::graph_traits<boost::adjacency_list<>>::edge_iterator edgeIt, edgeEnd;
  boost::tie(edgeIt, edgeEnd) = boost::edges(subgraph);
  for (; edgeIt != edgeEnd; edgeIt++) {
    auto source = boost::source(*edgeIt, graph);
    auto target = boost::target(*edgeIt, graph);
    if (source >= assignments.size() || target >= assignments.size()) {
      continue;
    }
    auto sourceGraph = assignments[source];
    auto targetGraph = assignments[target];
    if (!boost::edge(sourceGraph, targetGraph, graph).second) {
      return false;
    }
  }
  return true;
}

static void
PrintState(std::vector<uint64_t> const &assignments,
           std::vector<std::vector<uint64_t>> const &possibleAssignments,
           std::string_view const &header) {
  std::cout << "<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<" << std::endl;
  std::cout << header << std::endl;
  for (size_t i = 0u; i < possibleAssignments.size(); i++) {
    std::cout << "Vertex " << i << ":" << std::endl;
    if (assignments.size() > i) {
      std::cout << "assigned to " << assignments[i] << std::endl;
    } else {
      std::cout << "unassigned" << std::endl;
    }
    std::cout << "Possible assignments: " << std::endl;
    for (auto const possibleAssignment : possibleAssignments[i]) {
      std::cout << possibleAssignment << " ";
    }
    if (possibleAssignments[i].empty()) {
      std::cout << "none";
    }
    std::cout << std::endl << std::endl;
  }
  std::cout << ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>" << std::endl << std::endl;
}

/**
 * @brief Finds an assignment for the subgraph given an initial assignments and
 * possible assignments.
 */
static bool
FindSubgraphImpl(boost::adjacency_list<> const &graph,
                 boost::adjacency_list<> const &subgraph,
                 std::vector<uint64_t> &assignments,
                 std::vector<std::vector<uint64_t>> possibleAssignments) {
  if (!DoAssignedEdgesExist(graph, subgraph, assignments)) {
    return false;
  }

  auto nextVertex = assignments.size();
  if (nextVertex == subgraph.m_vertices.size()) {
    return true;
  }

  // uncomment to debug
  // PrintState(assignments, possibleAssignments, "FindSubgraphImpl: Begin");

  UpdatePossibleAssignments(graph, subgraph, possibleAssignments);

  auto &possibleAssignmentsForVertex = possibleAssignments.at(nextVertex);
  while (!possibleAssignmentsForVertex.empty()) {
    auto assignedVertex = possibleAssignmentsForVertex.begin();
    if (std::find(assignments.begin(), assignments.end(), *assignedVertex) ==
        assignments.end()) {
      assignments.push_back(*assignedVertex);
      auto oldPossibleAssignments = possibleAssignmentsForVertex;
      // assignedVertex is the only possibility
      possibleAssignmentsForVertex = {*assignedVertex};
      if (FindSubgraphImpl(graph, subgraph, assignments, possibleAssignments))
        return true;

      // restore state for nextVertex
      assignments.pop_back();
      possibleAssignmentsForVertex = oldPossibleAssignments;
    }
    assignedVertex = possibleAssignmentsForVertex.erase(assignedVertex);
    UpdatePossibleAssignments(graph, subgraph, possibleAssignments);
  }
  return false;
}
std::optional<std::vector<uint64_t>>
FindSubgraph(boost::adjacency_list<> const &graph,
             boost::adjacency_list<> const &subgraph) {
  auto numVertexGraph = graph.m_vertices.size();
  auto numVertexSubgraph = subgraph.m_vertices.size();

  std::vector<uint64_t> assignments;

  std::vector<uint64_t> allVerticesInGraph(numVertexGraph);
  std::iota(allVerticesInGraph.begin(), allVerticesInGraph.end(), 0u);
  std::vector<std::vector<uint64_t>> possibleAssignments(
      subgraph.m_vertices.size(), allVerticesInGraph);

  auto isFound =
      FindSubgraphImpl(graph, subgraph, assignments, possibleAssignments);
  if (isFound)
    return assignments;
  else
    return std::nullopt;
};
} // namespace GraphAlgos