#ifndef VERY_BASIC_ASCII_GRAPHICS_API_INCLUDE_ANIMATION_ANIMATION_ENGINE_HPP
#define VERY_BASIC_ASCII_GRAPHICS_API_INCLUDE_ANIMATION_ANIMATION_ENGINE_HPP

#include "geometry/graph.hpp"
#include "geometry/mesh.hpp"
#include "output/screen.hpp"

#include <cmath>
#include <functional>
#include <utility>
#include <windows.h>

/// @brief can use an ASCII screen to draw and animate things.
/// takes a setup function, which is run exactly once at the start of the
/// animation sequence, and a loop function, which is run every single frame
class AnimationEngine {
public:
  using AnimFunc = std::function<void(AnimationEngine *)>;

  AnimationEngine(Screen &, AnimFunc, AnimFunc, float = 60.0F);
  void drawLine(V3F, V3F, float = 3, char = '#');
  void drawGraph(GV3F, float = 3, char = '#');
  [[noreturn]] void run();
  [[nodiscard]] float frameRate() const;
  [[nodiscard]] float frameTime() const;
  Screen &screen();
  static void delay(DWORD milliseconds);

private:
  Screen &screen_;
  AnimFunc setup_, loop_;
  float frameRate_;
};

#endif // VERY_BASIC_ASCII_GRAPHICS_API_INCLUDE_ANIMATION_ANIMATION_ENGINE_HPP
