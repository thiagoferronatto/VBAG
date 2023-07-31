#ifndef VERY_BASIC_ASCII_GRAPHICS_API_INCLUDE_GRAPH_HPP
#define VERY_BASIC_ASCII_GRAPHICS_API_INCLUDE_GRAPH_HPP

#include "geometry/object.hpp"

#include <cassert>
#include <cstdio>
#include <list>
#include <vector>

/// @brief represents a graph through an adjacency list
/// @tparam T the type that each vertex contains
template <typename T> class Graph : public Object {
public:
  Graph(const std::string &name) : Object(name){};

  auto addVertex(const T &position) {
    vertices_.emplace_back(position);
    adjacencyLists_.emplace_back();
  }

  std::enable_if_t<std::is_same_v<T, V3F>, void> addVertex(float x, float y,
                                                           float z) {
    addVertex({x, y, z});
  }

  auto addEdge(size_t vertex1, size_t vertex2) {
    assert(vertex1 < vertices_.size() && vertex2 < vertices_.size());
    adjacencyLists_[vertex1].push_back(vertex2);
    adjacencyLists_[vertex2].push_back(vertex1);
  }

  [[maybe_unused]] [[nodiscard]] const auto &vertices() const {
    return vertices_;
  }

  auto &vertices() { return vertices_; }

  [[maybe_unused]] [[nodiscard]] const auto &edges(size_t vertex) const {
    return adjacencyLists_[vertex];
  }
  auto &edges(size_t vertex) { return adjacencyLists_[vertex]; }

  [[nodiscard]] auto order() const { return vertices_.size(); }

  static Graph<V3F> cube(const std::string &name) {
    static constexpr size_t a = 0, b = 1, c = 2, d = 3, e = 4, f = 5, g = 6,
                            h = 7;
    Graph<V3F> graph{name};
    graph.addVertex(-1, 1, 1);
    graph.addVertex(-1, -1, 1);
    graph.addVertex(1, -1, 1);
    graph.addVertex(1, 1, 1);
    graph.addVertex(1, 1, -1);
    graph.addVertex(-1, 1, -1);
    graph.addVertex(-1, -1, -1);
    graph.addVertex(1, -1, -1);
    graph.addEdge(a, b);
    graph.addEdge(a, d);
    graph.addEdge(a, f);
    graph.addEdge(b, c);
    graph.addEdge(b, g);
    graph.addEdge(c, d);
    graph.addEdge(c, h);
    graph.addEdge(d, e);
    graph.addEdge(e, f);
    graph.addEdge(e, h);
    graph.addEdge(f, g);
    graph.addEdge(g, h);
    return graph;
  }

private:
  std::vector<T> vertices_;
  std::vector<std::list<size_t>> adjacencyLists_;
};

using GV3F = Graph<V3F>;

#endif // VERY_BASIC_ASCII_GRAPHICS_API_INCLUDE_GRAPH_HPP
