#include "geometry/mesh.hpp"

[[nodiscard]] const std::vector<V3F> &Mesh::vertices() const {
  return vertices_;
}

[[nodiscard]] const std::vector<V3F> &Mesh::normals() const { return normals_; }

[[nodiscard]] const std::vector<Mesh::Triangle> &Mesh::triangles() const {
  return triangles_;
}

Mesh::Triangle::Triangle(const Mesh *parent, const Triplet<size_t> &vertices)
    : parent_{parent}, first{vertices.first}, second{vertices.second},
      third{vertices.third} {}

[[nodiscard]] V3F Mesh::Triangle::arealCoordinates(const V3F &P) const {
  auto A{parent_->vertices()[first]}, B{parent_->vertices()[second]},
      C{parent_->vertices()[third]};
  const auto ap{P - A}, ab{B - A}, ac{C - A};
  const auto den{ab.cross(ac).length()};
  const auto a{(C - B).cross(P - B).length() / den},
      b{ac.cross(ap).length() / den}, c{ab.cross(ap).length() / den};
  return {a, b, c};
}
