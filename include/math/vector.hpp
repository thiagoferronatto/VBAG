#ifndef VERY_BASIC_ASCII_GRAPHICS_API_INCLUDE_VECTOR_HPP
#define VERY_BASIC_ASCII_GRAPHICS_API_INCLUDE_VECTOR_HPP

#include "math/matrix.hpp"
#include "util/floating_point_comparison.hpp"
#include <cmath>
#include <cstdint>

template <typename T, size_t dimension> class Vector {};

template <> struct Vector<float, 3> {
  [[nodiscard]] auto dot(const Vector &other) const {
    return x * other.x + y * other.y + z * other.z;
  }

  [[nodiscard]] auto cross(const Vector &other) const {
    return Vector{y * other.z - z * other.y, z * other.x - x * other.z,
                  x * other.y - y * other.x};
  }

  [[nodiscard]] auto operator-() const { return Vector{-x, -y, -z}; }

  [[nodiscard]] auto operator+(const Vector &other) const {
    return Vector{x + other.x, y + other.y, z + other.z};
  }

  [[nodiscard]] auto operator-(const Vector &other) const {
    return Vector{x - other.x, y - other.y, z - other.z};
  }

  [[nodiscard]] auto operator*(float scalar) const {
    return Vector{scalar * x, scalar * y, scalar * z};
  }

  [[nodiscard]] friend auto operator*(float scalar, const Vector &v) {
    return v * scalar;
  }

  [[nodiscard]] auto operator/(float scalar) const {
    return Vector{x / scalar, y / scalar, z / scalar};
  }

  // performs component-wise division. it's useful in some cases.
  [[nodiscard]] auto operator/(const Vector &other) const {
    return Vector{x / other.x, y / other.y, z / other.z};
  }

  Vector &operator+=(const Vector &other) { return *this = *this + other; }
  Vector &operator-=(const Vector &other) { return *this = *this - other; }
  Vector &operator*=(float scalar) { return *this = *this * scalar; }
  Vector &operator/=(float scalar) { return *this = *this / scalar; }
  Vector &operator/=(const Vector &other) { return *this = *this / other; }

  [[nodiscard]] auto length() const { return std::sqrt(dot(*this)); }

  [[nodiscard]] auto normalized() const {
    if (isZero())
      return Vector{};
    return operator/(length());
  }

  void normalize() {
    if (isZero())
      return;
    *this = normalized();
  }

  [[nodiscard]] bool isZero() const {
    return ::isZero(x) && ::isZero(y) && ::isZero(z);
  }

  static constexpr auto right() { return Vector{1, 0, 0}; }
  static constexpr auto up() { return Vector{0, 1, 0}; }
  static constexpr auto forward() { return Vector{0, 0, -1}; }

  float x, y, z;
};

using V3F = Vector<float, 3>;

#endif // VERY_BASIC_ASCII_GRAPHICS_API_INCLUDE_VECTOR_HPP
