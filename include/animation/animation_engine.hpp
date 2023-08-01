#ifndef VERY_BASIC_ASCII_GRAPHICS_API_INCLUDE_ANIMATION_ANIMATION_ENGINE_HPP
#define VERY_BASIC_ASCII_GRAPHICS_API_INCLUDE_ANIMATION_ANIMATION_ENGINE_HPP

#include <cmath>
#include <functional>
#include <utility>
#include <windows.h>

#include "geometry/graph.hpp"
#include "geometry/scene.hpp"
#include "graphics/camera.hpp"
#include "output/screen.hpp"

/// @brief can use an ASCII screen to draw and animate things.
/// takes a setup function, which is run exactly once at the start of the
/// animation sequence, and a loop function, which is run every single frame
class AnimationEngine {
  // TODO: add a setScene method, allowing for dynamic scene changing
public:
  using AnimFunc = std::function<void(AnimationEngine *)>;

  AnimationEngine(Screen &, AnimFunc, AnimFunc, Scene, float = 60.0F);
  void drawLine(V3F, V3F, float = 3, char = '#');
  void drawGraph(const GV3F *, float = 3, char = '#');
  void draw(float = 3, char = '#');
  [[noreturn]] void run();
  [[nodiscard]] float frameRate() const;
  [[nodiscard]] float frameTime() const;
  [[nodiscard]] Screen &screen();
  [[nodiscard]] Camera &camera();
  static void delay(float);
  [[nodiscard]] float deltaTime() const;

private:
  Screen &screen_;
  Scene scene_;
  AnimFunc setup_, loop_;
  float frameRate_, deltaTime_{};
};

#endif // VERY_BASIC_ASCII_GRAPHICS_API_INCLUDE_ANIMATION_ANIMATION_ENGINE_HPP
