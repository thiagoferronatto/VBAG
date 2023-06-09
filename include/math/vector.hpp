#ifndef VERY_BASIC_ASCII_GRAPHICS_API_INCLUDE_VECTOR_HPP
#define VERY_BASIC_ASCII_GRAPHICS_API_INCLUDE_VECTOR_HPP

#include <cmath>
#include <cstdint>

template <typename T, size_t dimension> class Vector {};

template <> struct Vector<float, 3> {
  [[nodiscard]] auto dot(const Vector &other) const {
    return x * other.x + y * other.y + z * other.z;
  }

  auto operator+(const Vector &other) const {
    return Vector{x + other.x, y + other.y, z + other.z};
  }

  auto operator-(const Vector &other) const {
    return Vector{x - other.x, y - other.y, z - other.z};
  }

  auto operator*(float scalar) const {
    return Vector{scalar * x, scalar * y, scalar * z};
  }

  friend auto operator*(float scalar, const Vector &v) { return v * scalar; }

  auto operator/(float scalar) const {
    return Vector{x / scalar, y / scalar, z / scalar};
  }

  // performs component-wise division. it's useful in some cases.
  auto operator/(const Vector &other) const {
    return Vector{x / other.x, y / other.y, z / other.z};
  }

  [[nodiscard]] auto length() const { return std::sqrt(dot(*this)); }

  [[nodiscard]] auto versor() const { return operator/(length()); }

  float x, y, z;
};

using V3F = Vector<float, 3>;

#endif // VERY_BASIC_ASCII_GRAPHICS_API_INCLUDE_VECTOR_HPP
