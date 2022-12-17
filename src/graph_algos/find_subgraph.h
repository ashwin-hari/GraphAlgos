#include <boost/graph/adjacency_list.hpp>
#include <optional>

namespace GraphAlgos {
/**
 * @brief finds a  whether graph contains a subgraph that is isomorphic to
 * subgraph.
 *
 * @return an assignment of vertices in subgraph to  vertices if the subgrah is
 * found, and std::nullopt otherwise.
 */
std::optional<std::vector<uint64_t>>
FindSubgraph(boost::adjacency_list<> const &graph,
             boost::adjacency_list<> const &subgraph);
} // namespace GraphAlgos