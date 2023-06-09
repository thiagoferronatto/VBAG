#ifndef VERY_BASIC_ASCII_GRAPHICS_API_INCLUDE_GRAPH_HPP
#define VERY_BASIC_ASCII_GRAPHICS_API_INCLUDE_GRAPH_HPP

#include "math/vector.hpp"
#include <cassert>
#include <list>
#include <vector>

/// @brief represents a graph through an adjacency list
/// @tparam T the type that each vertex contains
template <typename T> class Graph {
public:
  Graph() = default;

  auto addVertex(const T &position) {
    vertices_.push_back(position);
    adjacencyLists_.emplace_back();
  }

  auto addEdge(size_t vertex1, size_t vertex2) {
    assert(vertex1 < vertices_.size() && vertex2 < vertices_.size());
    adjacencyLists_[vertex1].push_back(vertex2);
    adjacencyLists_[vertex2].push_back(vertex1);
  }

  [[maybe_unused]] const auto &vertices() const { return vertices_; }
  auto &vertices() { return vertices_; }

  [[maybe_unused]] const auto &edges(size_t vertex) const { return adjacencyLists_[vertex]; }
  auto &edges(size_t vertex) { return adjacencyLists_[vertex]; }

  auto order() const { return vertices_.size(); }

private:
  std::vector<T> vertices_;
  std::vector<std::list<size_t>> adjacencyLists_;
};

using GV3F = Graph<V3F>;

#endif // VERY_BASIC_ASCII_GRAPHICS_API_INCLUDE_GRAPH_HPP
