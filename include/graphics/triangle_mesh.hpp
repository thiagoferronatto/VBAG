#ifndef VERY_BASIC_ASCII_GRAPHICS_API_INCLUDE_GRAPHICS_TRIANGLE_MESH_HPP
#define VERY_BASIC_ASCII_GRAPHICS_API_INCLUDE_GRAPHICS_TRIANGLE_MESH_HPP

#include "geometry/object.hpp"
#include "math/vector.hpp"
#include <vector>

class TriangleMesh : public Object {
public:
  struct Triangle {
    size_t v1, v2, v3;
  };

  explicit TriangleMesh(std::string name) : Object(std::move(name)) {}

  void addVertex(V3F vertex) { vertices_.emplace_back(vertex); }

  void addVertex(float x, float y, float z) { addVertex({x, y, z}); }

  void addNormal(V3F normal) { normals_.emplace_back(normal.normalized()); }

  void addNormal(float x, float y, float z) { addNormal({x, y, z}); }

  void addTriangle(Triangle triangle) { triangles_.emplace_back(triangle); }

  void addTriangle(size_t v1, size_t v2, size_t v3) {
    addTriangle({v1, v2, v3});
  }

  [[nodiscard]] const auto &vertices() const { return vertices_; }
  [[nodiscard]] const auto &normals() const { return normals_; }
  auto &normals() { return normals_; }
  [[nodiscard]] const auto &triangles() const { return triangles_; }

private:
  std::vector<V3F> vertices_, normals_;
  std::vector<Triangle> triangles_;
};

#endif // VERY_BASIC_ASCII_GRAPHICS_API_INCLUDE_GRAPHICS_TRIANGLE_MESH_HPP
