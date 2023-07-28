#ifndef VERY_BASIC_ASCII_GRAPHICS_API_INCLUDE_GRAPH_HPP
#define VERY_BASIC_ASCII_GRAPHICS_API_INCLUDE_GRAPH_HPP

#include "geometry/object.hpp"

#include <cassert>
#include <list>
#include <vector>

/// @brief represents a graph through an adjacency list
/// @tparam T the type that each vertex contains
template <typename T> class Graph : public Object {
public:
  Graph() = default;

  auto addVertex(const T &position) {
    vertices_.emplace_back(position);
    adjacencyLists_.emplace_back();
  }

  auto addEdge(size_t vertex1, size_t vertex2) {
    assert(vertex1 < vertices_.size() && vertex2 < vertices_.size());
    adjacencyLists_[vertex1].push_back(vertex2);
    adjacencyLists_[vertex2].push_back(vertex1);
  }

  [[maybe_unused]] const auto &vertices() const { return vertices_; }
  auto &vertices() { return vertices_; }

  [[maybe_unused]] const auto &edges(size_t vertex) const {
    return adjacencyLists_[vertex];
  }
  auto &edges(size_t vertex) { return adjacencyLists_[vertex]; }

  auto order() const { return vertices_.size(); }

  void rotateInPlace(V3F eulerAngles) override {
    V3F center{};
    for (const auto &vertex : vertices_)
      center += vertex;
    center /= order();
    center.transformAsPoint(transform_);
    translate(-center);
    rotate(eulerAngles);
    translate(center);
  }

  void rotateInPlace(float x, float y, float z) override {
    rotateInPlace({x, y, z});
  }

private:
  std::vector<T> vertices_;
  std::vector<std::list<size_t>> adjacencyLists_;
};

using GV3F = Graph<V3F>;

#endif // VERY_BASIC_ASCII_GRAPHICS_API_INCLUDE_GRAPH_HPP
