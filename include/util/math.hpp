#ifndef VERY_BASIC_ASCII_GRAPHICS_API_INCLUDE_UTIL_MATH_HPP
#define VERY_BASIC_ASCII_GRAPHICS_API_INCLUDE_UTIL_MATH_HPP

#include <cmath>
#include <concepts>

#undef max
#undef min

namespace vbag {

/// @tparam T The floating-point type of the values `a` and `b`.
/// @brief Compares two floating-point values for approximate equality.
///
/// @param a The first floating-point value to compare.
/// @param b The second floating-point value to compare.
/// @param epsilon The small epsilon value to use for the comparison (default:
/// 1e-5).
/// @return True if the absolute difference between `a` and `b` is less than
/// `epsilon`, false otherwise.
template <std::floating_point T>
inline constexpr auto areEqual(T a, T b, T epsilon = 1e-5) {
  return std::abs(a - b) < epsilon;
}

/// @tparam T The floating-point type of the value `x`.
/// @brief Checks if a floating-point value is effectively zero.
///
/// @param x The floating-point value to check for being effectively zero.
/// @param epsilon The small epsilon value to use for the comparison (default:
/// 1e-5).
/// @return True if the absolute difference between `x` and zero is less than
/// `epsilon`, false otherwise.
template <std::floating_point T>
inline constexpr auto isZero(T x, T epsilon = 1e-5) {
  return areEqual(x, T{}, epsilon);
}

template <typename T>
  requires std::totally_ordered<T>
inline constexpr auto(min)(const T &a, const T &b) {
  return a < b ? a : b;
}

template <typename T>
  requires std::totally_ordered<T>
inline constexpr auto(max)(const T &a, const T &b) {
  return a > b ? a : b;
}

} // namespace vbag

#endif // VERY_BASIC_ASCII_GRAPHICS_API_INCLUDE_UTIL_MATH_HPP
