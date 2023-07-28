#ifndef VERY_BASIC_ASCII_GRAPHICS_API_INCLUDE_GEOMETRY_MESH_HPP
#define VERY_BASIC_ASCII_GRAPHICS_API_INCLUDE_GEOMETRY_MESH_HPP

#include "data/triplet.hpp"
#include "math/vector.hpp"
#include <vector>

class Mesh {
public:
  class Triangle;

  [[nodiscard]] const std::vector<V3F> &vertices() const;
  [[nodiscard]] const std::vector<V3F> &normals() const;
  [[nodiscard]] const std::vector<Triangle> &triangles() const;

private:
  std::vector<V3F> vertices_;
  std::vector<V3F> normals_;
  std::vector<Triangle> triangles_;
};

class Mesh::Triangle {
public:
  [[nodiscard]] V3F arealCoordinates(const V3F &) const;

  size_t first, second, third;

private:
  friend Mesh;

  Triangle(const Mesh *, const Triplet<size_t> &);

  const Mesh *parent_;
};

#endif // VERY_BASIC_ASCII_GRAPHICS_API_INCLUDE_GEOMETRY_MESH_HPP
