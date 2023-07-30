#ifndef VERY_BASIC_ASCII_GRAPHICS_API_INCLUDE_UTIL_FLOATING_POINT_COMPARISON_HPP
#define VERY_BASIC_ASCII_GRAPHICS_API_INCLUDE_UTIL_FLOATING_POINT_COMPARISON_HPP

#include <cmath>

template <typename T>
inline constexpr auto areEqual(T a, T b, T epsilon = 1e-5) {
  return std::abs(a - b) < epsilon;
}

template <typename T> inline constexpr auto isZero(T x, T epsilon = 1e-5) {
  return areEqual(x, T{}, epsilon);
}

#endif // VERY_BASIC_ASCII_GRAPHICS_API_INCLUDE_UTIL_FLOATING_POINT_COMPARISON_HPP
