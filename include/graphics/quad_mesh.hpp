#ifndef VERY_BASIC_ASCII_GRAPHICS_API_INCLUDE_GRAPHICS_QUAD_MESH_HPP
#define VERY_BASIC_ASCII_GRAPHICS_API_INCLUDE_GRAPHICS_QUAD_MESH_HPP

#include "graphics/triangle_mesh.hpp"

class QuadMesh : public Object {
public:
  struct Quad {
    size_t v1, v2, v3, v4;
  };

public:
  explicit QuadMesh(const std::string &name) : Object(name) {}

  QuadMesh(const TriangleMesh &triangleMesh)
      : Object(triangleMesh.name() + "_as_quad_mesh") {
    for (const auto &vertex : triangleMesh.vertices())
      addVertex(vertex);
    for (const auto &normal : triangleMesh.normals())
      addNormal(normal);
    for (const auto &triangle : triangleMesh.triangles())
      // HACK: a degenerate quad, this is ridiculous but 100% functional
      addQuad(triangle.v1, triangle.v2, triangle.v3, triangle.v1);
  }

  void addVertex(V3F vertex) { vertices_.emplace_back(vertex); }

  void addVertex(float x, float y, float z) { addVertex({x, y, z}); }

  void addNormal(V3F normal) { normals_.emplace_back(normal.normalized()); }

  void addNormal(float x, float y, float z) { addNormal({x, y, z}); }

  void addQuad(Quad quad) { quads_.emplace_back(quad); }

  void addQuad(size_t v1, size_t v2, size_t v3, size_t v4) {
    addQuad({v1, v2, v3, v4});
  }

  [[nodiscard]] const auto &vertices() const { return vertices_; }
  [[nodiscard]] const auto &normals() const { return normals_; }
  auto &normals() { return normals_; }
  [[nodiscard]] const auto &faces() const { return quads_; }

  [[nodiscard]] TriangleMesh asTriangleMesh() const {
    TriangleMesh triangleMesh{name_ + "_as_triangle_mesh"};
    for (const auto &vertex : vertices_)
      triangleMesh.addVertex(vertex);
    for (const auto &normal : normals_)
      triangleMesh.addNormal(normal);
    for (const auto &quad : quads_) {
      triangleMesh.addTriangle(quad.v1, quad.v2, quad.v3);
      triangleMesh.addTriangle(quad.v1, quad.v3, quad.v4);
    }
    return triangleMesh;
  }

private:
  std::vector<V3F> vertices_, normals_;
  std::vector<Quad> quads_;
};

#endif // VERY_BASIC_ASCII_GRAPHICS_API_INCLUDE_GRAPHICS_QUAD_MESH_HPP
