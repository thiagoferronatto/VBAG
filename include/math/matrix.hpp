#ifndef VERY_BASIC_ASCII_GRAPHICS_API_INCLUDE_MATH_MATRIX_HPP
#define VERY_BASIC_ASCII_GRAPHICS_API_INCLUDE_MATH_MATRIX_HPP

#include <algorithm>
#include <cmath>
#include <numeric>
#include <type_traits>

// do not use this for huge matrices, may cause a stack overflow
// ideally this should be employed for 3x3 or 4x4 matrices
template <typename T, size_t h, size_t w> class Matrix {
public:
  T operator()(size_t row, size_t col) const { return data[row * w + col]; }
  T &operator()(size_t row, size_t col) { return data[row * w + col]; }

  Matrix operator-() const {
    return unaryOperationResult(*this, std::negate{});
  }

  Matrix operator+(const Matrix &other) const {
    return binaryOperationResult(*this, other, std::plus{});
  }

  Matrix operator-(const Matrix &other) const { return operator+(-other); }

  Matrix operator*(T scalar) const {
    return unaryOperationResult(
        *this, [scalar](T element) { return scalar * element; });
  }

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

  Matrix operator/(T scalar) const { return operator*(T{1} / scalar); }

  [[nodiscard]] size_t height() const { return h; }
  [[nodiscard]] size_t width() const { return w; }

  T data[h * w];

private:
  template <typename Op>
  static Matrix binaryOperationResult(const Matrix &a, const Matrix &b, Op op) {
    Matrix result;
    std::transform(a.data, a.data + a.height() * a.width(), b.data, result.data,
                   op);
    return result;
  }

  template <typename Op>
  static Matrix unaryOperationResult(const Matrix &m, Op op) {
    Matrix result;
    std::transform(m.data, m.data + m.height() * m.width(), result.data, op);
    return result;
  }
};

template <typename T> using M4 = Matrix<T, 4, 4>;

using M4F = M4<float>;

#endif // VERY_BASIC_ASCII_GRAPHICS_API_INCLUDE_MATH_MATRIX_HPP
