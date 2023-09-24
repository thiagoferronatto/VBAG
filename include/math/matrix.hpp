#ifndef VERY_BASIC_ASCII_GRAPHICS_API_INCLUDE_MATH_MATRIX_HPP
#define VERY_BASIC_ASCII_GRAPHICS_API_INCLUDE_MATH_MATRIX_HPP

#include <algorithm>
#include <cmath>
#include <numeric>
#include <type_traits>

namespace vbag {

/// @tparam T The data type of the matrix elements.
/// @tparam h The number of rows in the matrix.
/// @tparam w The number of columns in the matrix.
/// @class Matrix
/// @brief The Matrix class represents a generic matrix with fixed dimensions.
///
/// The Matrix class provides a generic implementation of a matrix with fixed
/// dimensions h (rows) and w (columns). It supports common matrix operations
/// such as addition, subtraction, scalar multiplication, matrix multiplication,
/// and element-wise negation. The matrix elements are stored in a
/// one-dimensional array.
///
/// @note Do not use for matrices of large (5+) dimensions.
template <typename T, size_t h, size_t w> class Matrix {
public:
  /// @brief Accesses the element at the specified row and column in the matrix.
  ///
  /// @param row The row index (0 to h-1) of the element to access.
  /// @param col The column index (0 to w-1) of the element to access.
  /// @return A constant reference to the element at the specified row and
  /// column in the matrix.
  [[nodiscard]] T operator()(size_t row, size_t col) const {
    return data[row * w + col];
  }

  /// @brief Accesses the element at the specified row and column in the matrix.
  ///
  /// @param row The row index (0 to h-1) of the element to access.
  /// @param col The column index (0 to w-1) of the element to access.
  /// @return A reference to the element at the specified row and column in the
  /// matrix.
  T &operator()(size_t row, size_t col) { return data[row * w + col]; }

  /// @brief Accesses the element at the specified index in the matrix
  /// (row-major order).
  ///
  /// @param index The linear index (0 to h*w-1) of the element to access.
  /// @return A constant reference to the element at the specified index in the
  /// matrix.
  [[nodiscard]] T operator[](size_t index) const { return data[index]; }

  /// @brief Accesses the element at the specified index in the matrix
  /// (row-major order).
  ///
  /// @param index The linear index (0 to h*w-1) of the element to access.
  /// @return A reference to the element at the specified index in the matrix.
  T &operator[](size_t index) { return data[index]; }

  /// @brief Negates all elements of the matrix (element-wise negation).
  ///
  /// @return A new matrix with all elements negated.
  Matrix operator-() const {
    return unaryOperationResult(*this, [](T elem) { return -elem; });
  }

  /// @brief Adds another matrix to this matrix (element-wise addition).
  ///
  /// @param other The matrix to add to this matrix.
  /// @return A new matrix resulting from the element-wise addition.
  Matrix operator+(const Matrix &other) const {
    return binaryOperationResult(*this, other, std::plus{});
  }

  /// @brief Subtracts another matrix from this matrix (element-wise
  /// subtraction).
  ///
  /// @param other The matrix to subtract from this matrix.
  /// @return A new matrix resulting from the element-wise subtraction.
  Matrix operator-(const Matrix &other) const { return operator+(-other); }

  /// @brief Multiplies the matrix by a scalar (element-wise scalar
  /// multiplication).
  ///
  /// @param scalar The scalar value to multiply the matrix by.
  /// @return A new matrix resulting from the element-wise scalar
  /// multiplication.
  Matrix operator*(T scalar) const {
    return unaryOperationResult(
        *this, [scalar](T element) { return scalar * element; });
  }

  /// @brief Multiplies the matrix by another matrix (matrix multiplication).
  ///
  /// The number of columns in this matrix (w) must be equal to the number of
  /// rows in the other matrix (otherWidth).
  ///
  /// @tparam otherWidth The number of columns in the other matrix.
  /// @param other The matrix to multiply with this matrix.
  /// @return A new matrix resulting from the matrix multiplication.
  template <size_t otherWidth>
  Matrix<T, h, otherWidth>
  operator*(const Matrix<T, w, otherWidth> &other) const {
    Matrix<T, h, otherWidth> result{};
    for (size_t i{}; i < h; ++i)
      for (size_t j{}; j < otherWidth; ++j)
        for (size_t k{}; k < w; ++k)
          result(i, j) += operator()(i, k) * other(k, j);
    return result;
  }

  /// @brief Divides the matrix by a scalar (element-wise scalar division).
  ///
  /// @param scalar The scalar value to divide the matrix by.
  /// @return A new matrix resulting from the element-wise scalar division.
  Matrix operator/(T scalar) const { return operator*(T{1} / scalar); }

  /// @brief Returns the number of rows in the matrix.
  ///
  /// @return The number of rows in the matrix.
  [[nodiscard]] size_t height() const { return h; }

  /// @brief Returns the number of columns in the matrix.
  ///
  /// @return The number of columns in the matrix.
  [[nodiscard]] size_t width() const { return w; }

  /// @brief Assumes this is a MV transform matrix.
  /// @return The transpose of the inverse of this matrix.
  Matrix transposeOfInverse() const {
    // TODO: make sure this is right
    const auto den{operator()(0, 2) * operator()(1, 1) * operator()(2, 0) -
                   operator()(0, 1) * operator()(1, 2) * operator()(2, 0) -
                   operator()(0, 2) * operator()(1, 0) * operator()(2, 1) +
                   operator()(0, 0) * operator()(1, 2) * operator()(2, 1) +
                   operator()(0, 1) * operator()(1, 0) * operator()(2, 2) -
                   operator()(0, 0) * operator()(1, 1) * operator()(2, 2)};
    // clang-format off
    return {
        (operator()(1,2)*operator()(2,1)-operator()(1,1)*operator()(2,2))  / den,  (-operator()(1,2)*operator()(2,0)+operator()(1,0)*operator()(2,2)) / den, (operator()(1,1)*operator()(2,0)-operator()(1,0)*operator()(2,1))  / den, 0,
        (-operator()(0,2)*operator()(2,1)+operator()(0,1)*operator()(2,2)) / den,  (operator()(0,2)*operator()(2,0)-operator()(0,0)*operator()(2,2))  / den, (-operator()(0,1)*operator()(2,0)+operator()(0,0)*operator()(2,1)) / den, 0,
        (operator()(0,2)*operator()(1,1)-operator()(0,1)*operator()(1,2))  / den,  (-operator()(0,2)*operator()(1,0)+operator()(0,0)*operator()(1,2)) / den, (operator()(0,1)*operator()(1,0)-operator()(0,0)*operator()(1,1))  / den, 0,
        0,                                                                         0,                                                                        0,                                                                        1
    };
    // clang-format on
  }

  /// @brief The one-dimensional array storing the matrix elements in row-major
  /// order.
  T data[h * w];

private:
  /// @tparam Op The type of the binary operation functor.
  /// @param a The first input matrix for the binary operation.
  /// @param b The second input matrix for the binary operation.
  /// @param op The binary operation functor that defines the operation to be
  /// performed.
  /// @return A new matrix resulting from the element-wise binary operation.
  template <typename Op>
  static Matrix binaryOperationResult(const Matrix &a, const Matrix &b, Op op) {
    Matrix result;
    std::transform(a.data, a.data + a.height() * a.width(), b.data, result.data,
                   op);
    return result;
  }

  /// @tparam Op The type of the unary operation functor.
  /// @param m The input matrix for the unary operation.
  /// @param op The unary operation functor that defines the operation to be
  /// performed.
  /// @return A new matrix resulting from the element-wise unary operation.
  template <typename Op>
  static Matrix unaryOperationResult(const Matrix &m, Op op) {
    Matrix result;
    std::transform(m.data, m.data + m.height() * m.width(), result.data, op);
    return result;
  }
};

/// @brief Type alias for a 4x4 matrix with elements of type T.
template <typename T> using M4 = Matrix<T, 4, 4>;

/// @brief Type alias for a 4x4 matrix with elements of type float.
using M4F = M4<float>;

} // namespace vbag

#endif // VERY_BASIC_ASCII_GRAPHICS_API_INCLUDE_MATH_MATRIX_HPP
