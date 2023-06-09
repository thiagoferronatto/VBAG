#ifndef VERY_BASIC_ASCII_GRAPHICS_API_INCLUDE_GRAPHICS_RAY_HPP
#define VERY_BASIC_ASCII_GRAPHICS_API_INCLUDE_GRAPHICS_RAY_HPP

#include "geometry/mesh.hpp"
#include "math/vector.hpp"
#include "util/floating_point_comparison.hpp"

#include <cstdint>
#include <optional>
#include <tuple>

template <typename T, size_t dimension> class Ray {};

template <> struct Ray<float, 3> {
  [[nodiscard]] std::tuple<bool, V3F, V3F>
  intersection(const Mesh &mesh) const {
    auto maxZ{-std::numeric_limits<float>::infinity()};
    V3F intersectPoint, normal, closestHit;
    auto didHit{false};
    for (auto &triangle : mesh.triangles()) {
      const auto a{mesh.vertices()[triangle.first]},
          b{mesh.vertices()[triangle.second]},
          c{mesh.vertices()[triangle.third]};
      const auto n = (b - a).cross(c - a).versor();
      const auto dDotN{direction_.dot(n)};
      if (areEqual(dDotN, 0.0F))
        continue;
      intersectPoint = source_ + ((a - source_).dot(n) / dDotN) * n;
      const auto areal{triangle.arealCoordinates(intersectPoint)};
      if (areal.x < 0 || areal.x > 1 || areal.y < 0 || areal.y > 1 ||
          areal.z < 0 || areal.z > 1)
        continue;
      didHit = true;
      if (intersectPoint.z < maxZ)
        continue;
      maxZ = intersectPoint.z;
      closestHit = intersectPoint;
      const auto an{mesh.normals()[triangle.first]},
          bn{mesh.normals()[triangle.second]},
          cn{mesh.normals()[triangle.third]};
      normal = areal.x * an + areal.y * bn + areal.z * cn;
    }
    return std::tie(didHit, closestHit, normal);
  }

  V3F source_, direction_;
};

using R3F = Ray<float, 3>;

#endif // VERY_BASIC_ASCII_GRAPHICS_API_INCLUDE_GRAPHICS_RAY_HPP
