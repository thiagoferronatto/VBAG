#ifndef VERY_BASIC_ASCII_GRAPHICS_API_INCLUDE_VECTOR_HPP
#define VERY_BASIC_ASCII_GRAPHICS_API_INCLUDE_VECTOR_HPP

#include "math/matrix.hpp"
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

  [[nodiscard]] auto transformedAsPoint(const M4F &matrix) const {
    Matrix<float, 4, 1> extended{x, y, z, 1};
    auto result{matrix * extended};
    auto den{result.data[3]};
    return Vector{result.data[0] / den, result.data[1] / den,
                  result.data[2] / den};
  }

  auto transformAsPoint(const M4F &matrix) {
    *this = transformedAsPoint(matrix);
  }

  [[maybe_unused]] [[nodiscard]] auto transformedAsVector(const M4F &matrix) const {
    Matrix<float, 4, 1> extended{x, y, z, 0};
    auto result{matrix * extended};
    return Vector{result.data[0], result.data[1], result.data[2]};
  }

  auto transformAsVector(const M4F &matrix) {
    Matrix<float, 4, 1> extended{x, y, z, 0};
    auto result{matrix * extended};
    x = result.data[0];
    y = result.data[1];
    z = result.data[2];
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

  [[nodiscard]] auto versor() const { return operator/(length()); }

  void normalize() { *this = versor(); }

  [[nodiscard]] auto isZero() const {
    constexpr auto eps{std::numeric_limits<float>::epsilon()};
    const auto xEps{eps * x}, yEps{eps * y}, zEps{eps * z};
    return std::abs(x) < std::abs(xEps) && //
           std::abs(y) < std::abs(yEps) && //
           std::abs(z) < std::abs(zEps);
  }

  float x, y, z;
};

using V3F = Vector<float, 3>;

#endif // VERY_BASIC_ASCII_GRAPHICS_API_INCLUDE_VECTOR_HPP
