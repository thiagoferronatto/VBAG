#ifndef VERY_BASIC_ASCII_GRAPHICS_API_INCLUDE_GRAPH_HPP
#define VERY_BASIC_ASCII_GRAPHICS_API_INCLUDE_GRAPH_HPP

#include "geometry/object.hpp"

#include <cassert>
#include <cstdio>
#include <list>
#include <vector>

/// @tparam T The type contained by each vertex.
/// @class Graph
/// @brief A class template representing a graph data structure.
///
/// The Graph class represents a generic graph data structure with vertices and
/// edges. It supports adding vertices, adding edges, and provides various
/// utility functions for working with graphs.
template <typename T> class Graph : public Object {
public:
  /// @brief Constructs a Graph object with the given name.
  ///
  /// @param name The name of the graph.
  explicit Graph(const std::string &name) : Object(name){};

  /// @brief Adds a vertex to the graph.
  ///
  /// @param value The value of the vertex to be added.
  auto addVertex(const T &value) {
    vertices_.emplace_back(value);
    adjacencyLists_.emplace_back();
  }

  /// @brief Adds a vertex to the graph using individual x, y, and z
  /// coordinates.
  ///
  /// @param x The x-coordinate of the vertex.
  /// @param y The y-coordinate of the vertex.
  /// @param z The z-coordinate of the vertex.
  ///
  /// This overload is only available for T = V3F (3D vector).
  std::enable_if_t<std::is_same_v<T, V3F>, void> //
  addVertex(float x, float y, float z) {
    addVertex({x, y, z});
  }

  /// @brief Adds an edge between two vertices in the graph.
  ///
  /// @param vertex1 The index of the first vertex.
  /// @param vertex2 The index of the second vertex.
  auto addEdge(size_t vertex1, size_t vertex2) {
    assert(vertex1 < vertices_.size() && vertex2 < vertices_.size());
    adjacencyLists_[vertex1].push_back(vertex2);
    adjacencyLists_[vertex2].push_back(vertex1);
  }

  /// @brief Returns a constant reference to the vertices of the graph.
  ///
  /// @return A constant reference to the vector of vertices.
  [[maybe_unused]] [[nodiscard]] const auto &vertices() const {
    return vertices_;
  }

  /// @brief Returns a reference to the vertices of the graph.
  ///
  /// @return A reference to the vector of vertices.
  auto &vertices() { return vertices_; }

  /// @brief Returns a constant reference to the edges of a specific vertex in
  /// the graph.
  ///
  /// @param vertex The index of the vertex to get the edges for.
  /// @return A constant reference to the list of edges for the specified
  /// vertex.
  [[maybe_unused]] [[nodiscard]] const auto &edges(size_t vertex) const {
    return adjacencyLists_[vertex];
  }

  /// @brief Returns a reference to the edges of a specific vertex in the graph.
  ///
  /// @param vertex The index of the vertex to get the edges for.
  /// @return A reference to the list of edges for the specified vertex.
  auto &edges(size_t vertex) { return adjacencyLists_[vertex]; }

  /// @brief Returns the order of the graph (the number of vertices).
  ///
  /// @return The number of vertices in the graph.
  [[nodiscard]] auto order() const { return vertices_.size(); }

  /// @brief Creates and returns a graph representing a cube.
  ///
  /// The cube graph is a special case and is provided as a static function.
  ///
  /// @param name The name of the cube graph.
  /// @return A Graph<V3F> representing a cube.
  static Graph<V3F> cube(const std::string &name) {
    static constexpr size_t a{0}, b{1}, c{2}, d{3}, e{4}, f{5}, g{6}, h{7};
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
  std::vector<T> vertices_; ///< The vertices of the graph.
  std::vector<std::list<size_t>>
      adjacencyLists_; ///< The adjacency lists for each vertex.
};

using GV3F = Graph<V3F>;

#endif // VERY_BASIC_ASCII_GRAPHICS_API_INCLUDE_GRAPH_HPP
