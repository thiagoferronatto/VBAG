#ifndef VERY_BASIC_ASCII_GRAPHICS_API_INCLUDE_VECTOR_HPP
#define VERY_BASIC_ASCII_GRAPHICS_API_INCLUDE_VECTOR_HPP

#include "math/matrix.hpp"
#include "util/floating_point_comparison.hpp"
#include <cmath>
#include <cstdint>

/// @tparam T The data type of the vector elements.
/// @tparam dimension The dimension of the vector.
/// @class Vector
/// @brief The Vector class represents a generic vector with a fixed dimension.
///
/// The Vector class should provide a generic implementation of a vector with a
/// fixed dimension. It should support common vector operations such as dot
/// product, cross product, addition, subtraction, scalar multiplication,
/// normalization, and component-wise division. The vector elements should be
/// stored as public members x, y, and z for 3D vectors, and an additional w for
/// 4D vectors.
template <typename T, size_t dimension> class Vector {};

/// @brief Specialization of the Vector class for 3D vectors with float
/// elements.
///
/// The Vector specialization for 3D vectors with float elements provides
/// specific implementations for 3D vector operations and convenience functions
/// for the most common 3D directions (right, up, and forward).
template <> struct Vector<float, 3> {
  /// @brief Computes the dot product between this vector and another vector.
  ///
  /// @param other The other vector for the dot product computation.
  /// @return The dot product between this vector and the other vector.
  [[nodiscard]] auto dot(const Vector &other) const {
    return x * other.x + y * other.y + z * other.z;
  }

  /// @brief Computes the cross product between this vector and another vector.
  ///
  /// @param other The other vector for the cross product computation.
  /// @return The cross product (vector) between this vector and the other
  /// vector.
  [[nodiscard]] auto cross(const Vector &other) const {
    return Vector{y * other.z - z * other.y, z * other.x - x * other.z,
                  x * other.y - y * other.x};
  }

  /// @brief Negates all elements of the vector (element-wise negation).
  ///
  /// @return A new vector with all elements negated.
  [[nodiscard]] auto operator-() const { return Vector{-x, -y, -z}; }

  /// @brief Adds another vector to this vector (element-wise addition).
  ///
  /// @param other The vector to add to this vector.
  /// @return A new vector resulting from the element-wise addition.
  [[nodiscard]] auto operator+(const Vector &other) const {
    return Vector{x + other.x, y + other.y, z + other.z};
  }

  /// @brief Subtracts another vector from this vector (element-wise
  /// subtraction).
  ///
  /// @param other The vector to subtract from this vector.
  /// @return A new vector resulting from the element-wise subtraction.
  [[nodiscard]] auto operator-(const Vector &other) const {
    return Vector{x - other.x, y - other.y, z - other.z};
  }

  /// @brief Multiplies the vector by a scalar (element-wise scalar
  /// multiplication).
  ///
  /// @param scalar The scalar value to multiply the vector by.
  /// @return A new vector resulting from the element-wise scalar
  /// multiplication.
  [[nodiscard]] auto operator*(float scalar) const {
    return Vector{scalar * x, scalar * y, scalar * z};
  }

  /// @brief Multiplies the vector by a scalar (scalar-vector multiplication,
  /// commutative).
  ///
  /// @param scalar The scalar value to multiply the vector by.
  /// @param v The vector to be multiplied by the scalar.
  /// @return A new vector resulting from the scalar-vector multiplication.
  [[nodiscard]] friend auto operator*(float scalar, const Vector &v) {
    return v * scalar;
  }

  /// @brief Divides the vector by a scalar (element-wise scalar division).
  ///
  /// @param scalar The scalar value to divide the vector by.
  /// @return A new vector resulting from the element-wise scalar division.
  [[nodiscard]] auto operator/(float scalar) const {
    return Vector{x / scalar, y / scalar, z / scalar};
  }

  /// @brief Divides the vector by another vector (element-wise division).
  ///
  /// @param other The vector containing the scalar components for division.
  /// @return A new vector resulting from the element-wise division.
  [[nodiscard]] auto operator/(const Vector &other) const {
    return Vector{x / other.x, y / other.y, z / other.z};
  }

  /// @brief Adds another vector to this vector (element-wise addition,
  /// in-place).
  ///
  /// @param other The vector to add to this vector.
  /// @return A reference to this vector after the addition operation.
  Vector &operator+=(const Vector &other) { return *this = *this + other; }

  /// @brief Subtracts another vector from this vector (element-wise
  /// subtraction, in-place).
  ///
  /// @param other The vector to subtract from this vector.
  /// @return A reference to this vector after the subtraction operation.
  Vector &operator-=(const Vector &other) { return *this = *this - other; }

  /// @brief Multiplies the vector by a scalar (element-wise scalar
  /// multiplication, in-place).
  ///
  /// @param scalar The scalar value to multiply the vector by.
  /// @return A reference to this vector after the scalar multiplication
  /// operation.
  Vector &operator*=(float scalar) { return *this = *this * scalar; }

  /// @brief Divides the vector by a scalar (element-wise scalar division,
  /// in-place).
  ///
  /// @param scalar The scalar value to divide the vector by.
  /// @return A reference to this vector after the scalar division operation.
  Vector &operator/=(float scalar) { return *this = *this / scalar; }

  /// @brief Divides the vector by another vector (element-wise division,
  /// in-place).
  ///
  /// @param other The vector containing the scalar components for division.
  /// @return A reference to this vector after the element-wise division
  /// operation.
  Vector &operator/=(const Vector &other) { return *this = *this / other; }

  /// @brief Computes the magnitude of the vector.
  ///
  /// @return The magnitude of the vector.
  [[nodiscard]] auto magnitude() const { return std::sqrt(dot(*this)); }

  /// @brief Computes the normalized (unit) vector.
  ///
  /// If the vector is a zero vector, a zero vector is returned.
  ///
  /// @return The normalized (unit) vector or a zero vector if the vector is
  /// zero.
  [[nodiscard]] auto normalized() const {
    if (isZero())
      return Vector{};
    return operator/(magnitude());
  }

  /// @brief Normalizes this vector to become a unit vector.
  ///
  /// If the vector is a zero vector, it remains unchanged.
  void normalize() {
    if (isZero())
      return;
    *this = normalized();
  }

  /// @brief Checks if the vector is a zero vector (all elements are zero).
  ///
  /// @return True if the vector is a zero vector, false otherwise.
  [[nodiscard]] bool isZero() const {
    return ::isZero(x) && ::isZero(y) && ::isZero(z);
  }

  /// @brief Returns a constant vector representing the right direction
  /// (x-axis).
  ///
  /// @return The right direction vector.
  static constexpr auto right() { return Vector{1, 0, 0}; }

  /// @brief Returns a constant vector representing the up direction (y-axis).
  ///
  /// @return The up direction vector.
  static constexpr auto up() { return Vector{0, 1, 0}; }

  /// @brief Returns a constant vector representing the forward direction
  /// (negative z-axis).
  ///
  /// @return The forward direction vector.
  static constexpr auto forward() { return Vector{0, 0, -1}; }

  float x, y, z; ///< The vector elements (x, y, z) for 3D vectors.
};

/// @brief Type alias for a 3D vector with elements of type float.
using V3F = Vector<float, 3>;

#endif // VERY_BASIC_ASCII_GRAPHICS_API_INCLUDE_VECTOR_HPP
