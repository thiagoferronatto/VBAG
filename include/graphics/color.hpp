#ifndef VERY_BASIC_ASCII_GRAPHICS_API_INCLUDE_GRAPHICS_COLOR_HPP
#define VERY_BASIC_ASCII_GRAPHICS_API_INCLUDE_GRAPHICS_COLOR_HPP

#include <cmath>
#include <d3d9.h>

namespace vbag {

class RgbColor {
public:
  constexpr operator D3DCOLOR() const {
    auto cpy{clamped()};
    return D3DCOLOR_XRGB(int(255.0f * cpy.r), int(255.0f * cpy.g),
                         int(255.0f * cpy.b));
  }

  constexpr void clamp() {
    r = r <= 1 ? r : 1;
    g = g <= 1 ? g : 1;
    b = b <= 1 ? b : 1;
  }

  [[nodiscard]] constexpr RgbColor clamped() const {
    auto result{*this};
    result.clamp();
    return result;
  }

  static constexpr RgbColor red() { return {1, 0, 0}; }
  static constexpr RgbColor green() { return {0, 1, 0}; }
  static constexpr RgbColor blue() { return {0, 0, 1}; }
  static constexpr RgbColor yellow() { return {1, 1, 0}; }
  static constexpr RgbColor cyan() { return {0, 1, 1}; }
  static constexpr RgbColor magenta() { return {1, 0, 1}; }
  static constexpr RgbColor black() { return {0, 0, 0}; }
  static constexpr RgbColor white() { return {1, 1, 1}; }
  static constexpr RgbColor grey() { return {0.5, 0.5, 0.5}; }
  static constexpr RgbColor gray() { return grey(); }
  static constexpr RgbColor lightGrey() { return {0.75, 0.75, 0.75}; }
  static constexpr RgbColor lightGray() { return lightGrey(); }
  static constexpr RgbColor darkGrey() { return {0.25, 0.25, 0.25}; }
  static constexpr RgbColor darkGray() { return darkGrey(); }
  static constexpr RgbColor orange() { return {1, 0.5, 0}; }
  static constexpr RgbColor purple() { return {0.5, 0, 0.5}; }
  static constexpr RgbColor brown() { return {0.6, 0.4, 0.2}; }
  static constexpr RgbColor pink() { return {1, 0.6, 0.8}; }
  static constexpr RgbColor teal() { return {0, 0.5, 0.5}; }

  float r, g, b;
};

} // namespace vbag

#endif // VERY_BASIC_ASCII_GRAPHICS_API_INCLUDE_GRAPHICS_COLOR_HPP
