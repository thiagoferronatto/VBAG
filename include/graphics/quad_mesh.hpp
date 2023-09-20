#ifndef VERY_BASIC_ASCII_GRAPHICS_API_INCLUDE_GRAPHICS_QUAD_MESH_HPP
#define VERY_BASIC_ASCII_GRAPHICS_API_INCLUDE_GRAPHICS_QUAD_MESH_HPP

#include <unordered_set>

#include "graphics/triangle_mesh.hpp"

struct Edge {
  [[nodiscard]] bool operator==(const Edge &other) const {
    return v1 == other.v1 && v2 == other.v2 || v1 == other.v2 && v2 == other.v1;
  }

  size_t v1, v2;
  std::unordered_set<size_t> adjacentFaces;
  V3F midpoint;
};

template <> struct std::hash<Edge> {
  auto operator()(const Edge &edge) const noexcept {
    return hash<size_t>{}(edge.v1) ^ (hash<size_t>{}(edge.v2) << 1);
  }
};

class QuadMesh : public Object {
public:
  struct Quad {
    size_t v1, v2, v3, v4;
  };

private:
  struct Vertex {
    V3F position;
    std::unordered_set<size_t> adjacentFaces, adjacentEdges;
  };

  struct Face {
    Quad quad;
    std::unordered_set<size_t> adjacentEdges;
    V3F centroid;
  };

public:
  explicit QuadMesh(const std::string &name) : Object(name) {}

  QuadMesh(const TriangleMesh &triangleMesh)
      : Object(triangleMesh.name() + "_as_quad_mesh") {
    // TODO: add point to middle of triangle, make 3 quads, add them in
    _set_errno(ENOSYS); // errno for "function not implemented"
    exit(1);
  }

  void addVertex(V3F vertex) { vertices_.emplace_back(vertex); }

  void addVertex(float x, float y, float z) { addVertex({x, y, z}); }

  void addNormal(V3F normal) { normals_.emplace_back(normal.normalized()); }

  void addNormal(float x, float y, float z) { addNormal({x, y, z}); }

  void addQuad(Quad quad) {
    vertices_[quad.v1].adjacentFaces.emplace(faces_.size());
    vertices_[quad.v2].adjacentFaces.emplace(faces_.size());
    vertices_[quad.v3].adjacentFaces.emplace(faces_.size());
    vertices_[quad.v4].adjacentFaces.emplace(faces_.size());

    vertices_[quad.v1].adjacentEdges.emplace(edges_.size());
    vertices_[quad.v2].adjacentEdges.emplace(edges_.size());
    Face face{quad};
    face.adjacentEdges.emplace(edges_.size());
    Edge edge{quad.v1, quad.v2};
    edge.adjacentFaces.emplace(faces_.size());
    edge.midpoint =
        0.5f * (vertices_[quad.v1].position + vertices_[quad.v2].position);
    addEdge(std::move(edge));

    vertices_[quad.v2].adjacentEdges.emplace(edges_.size());
    vertices_[quad.v3].adjacentEdges.emplace(edges_.size());
    face.adjacentEdges.emplace(edges_.size());
    edge = {quad.v2, quad.v3};
    edge.adjacentFaces.emplace(faces_.size());
    edge.midpoint =
        0.5f * (vertices_[quad.v2].position + vertices_[quad.v3].position);
    addEdge(std::move(edge));

    vertices_[quad.v3].adjacentEdges.emplace(edges_.size());
    vertices_[quad.v4].adjacentEdges.emplace(edges_.size());
    face.adjacentEdges.emplace(edges_.size());
    edge = {quad.v3, quad.v4};
    edge.adjacentFaces.emplace(faces_.size());
    edge.midpoint =
        0.5f * (vertices_[quad.v3].position + vertices_[quad.v4].position);
    addEdge(std::move(edge));

    vertices_[quad.v4].adjacentEdges.emplace(edges_.size());
    vertices_[quad.v1].adjacentEdges.emplace(edges_.size());
    face.adjacentEdges.emplace(edges_.size());
    edge = {quad.v4, quad.v1};
    edge.adjacentFaces.emplace(faces_.size());
    edge.midpoint =
        0.5f * (vertices_[quad.v4].position + vertices_[quad.v1].position);
    addEdge(std::move(edge));

    face.centroid =
        0.25f * (vertices_[quad.v1].position + vertices_[quad.v2].position +
                 vertices_[quad.v3].position + vertices_[quad.v4].position);
    faces_.emplace_back(std::move(face));
  }

  void addQuad(size_t v1, size_t v2, size_t v3, size_t v4) {
    addQuad({v1, v2, v3, v4});
  }

  [[nodiscard]] const auto &vertices() const { return vertices_; }
  [[nodiscard]] const auto &normals() const { return normals_; }
  auto &normals() { return normals_; }
  [[nodiscard]] const auto &faces() const { return faces_; }

  [[nodiscard]] TriangleMesh asTriangleMesh() const {
    TriangleMesh triangleMesh{name_ + "_as_triangle_mesh"};
    for (const auto &vertex : vertices_)
      triangleMesh.addVertex(vertex.position);
    for (const auto &normal : normals_)
      triangleMesh.addNormal(normal);
    for (const auto &face : faces_) {
      triangleMesh.addTriangle(face.quad.v1, face.quad.v2, face.quad.v3);
      triangleMesh.addTriangle(face.quad.v1, face.quad.v3, face.quad.v4);
    }
    return triangleMesh;
  }

private:
  void addEdge(Edge &&edge) {
    if (std::find(edges_.begin(), edges_.end(), edge) == edges_.end())
      edges_.emplace_back(std::forward<Edge>(edge));
  }

  std::vector<Vertex> vertices_;
  std::vector<V3F> normals_;
  std::vector<Face> faces_;
  std::vector<Edge> edges_;
};

#endif // VERY_BASIC_ASCII_GRAPHICS_API_INCLUDE_GRAPHICS_QUAD_MESH_HPP
