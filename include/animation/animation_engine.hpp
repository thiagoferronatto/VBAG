#ifndef VERY_BASIC_ASCII_GRAPHICS_API_INCLUDE_ANIMATION_ANIMATION_ENGINE_HPP
#define VERY_BASIC_ASCII_GRAPHICS_API_INCLUDE_ANIMATION_ANIMATION_ENGINE_HPP

#include "math/graph.hpp"
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

  explicit AnimationEngine(Screen &screen, AnimFunc setup, AnimFunc loop,
                           float frameRate = 60.0F)
      : screen_{screen}, setup_{std::move(setup)}, loop_{std::move(loop)},
        frameRate_{frameRate} {}

  auto drawLine(V3F a, V3F b, float thickness = 3, char fill = '#') {
    a.z = b.z = 0.0F;
    const auto ab = b - a;
    const auto abn = ab.versor();
    V3F p{}, ap, proj;
    float t;
    const auto centerX{float(screen_.width()) / 2.0F};
    const auto centerY{float(screen_.height()) / 2.0F};
    // TODO: adjust bounding box to account for screen coords
    // const auto minX = std::min(std::min(a.x, b.x), 0.0F);
    // const auto minY = std::min(std::min(a.y, b.y), 0.0F);
    // const auto maxX = std::max(std::max(a.x, b.x), float{g_screenWidth});
    // const auto maxY = std::max(std::max(a.y, b.y), float{g_screenHeight});
    for (size_t i{}; i < screen_.height(); ++i) {
      p.y = float(screen_.height()) - float(i) - 1.0F - centerY;
      for (size_t j{}; float(j) < float(screen_.width()); ++j) {
        p.x = float(j) - centerX;
        ap = p - a;
        proj = (ap.dot(abn)) * abn;
        t = ((a + proj).x - a.x) / ab.x;
        if ((ap - proj).length() < thickness && 0.0F <= t && t <= 1.0F)
          screen_.set(i, j, fill);
      }
    }
  }

  auto drawGraph(GV3F g, float tx, float ty, float tz, float thickness = 3,
                 char fill = '#') {
    V3F center{};
    for (auto &vertex : g.vertices())
      center = center + vertex;
    center = center / static_cast<float>(g.order());

    for (auto &vertex : g.vertices())
      vertex = vertex - center;

    // rotation around x-axis
    float tmp;
    for (auto &vertex : g.vertices()) {
      tmp = vertex.y;
      vertex.y = vertex.y * std::cos(tx) - vertex.z * std::sin(tx);
      vertex.z = tmp * std::sin(tx) + vertex.z * std::cos(tx);
    }

    // rotation around y-axis
    for (auto &vertex : g.vertices()) {
      tmp = vertex.x;
      vertex.x = vertex.x * std::cos(ty) + vertex.z * std::sin(ty);
      vertex.z = vertex.z * std::cos(ty) - tmp * std::sin(ty);
    }

    // rotation around z-axis
    for (auto &vertex : g.vertices()) {
      tmp = vertex.x;
      vertex.x = vertex.x * std::cos(tz) - vertex.y * std::sin(tz);
      vertex.y = tmp * std::sin(tz) + vertex.y * std::cos(tz);
    }

    for (auto &vertex : g.vertices())
      vertex = vertex + center;

    for (size_t i{}; i < g.order(); ++i)
      for (auto elem : g.edges(i))
        drawLine(g.vertices()[i], g.vertices()[elem], thickness, fill);
  }

  [[noreturn]] auto run() {
    screen_.clear();
    setup_(this);
    for (;;) {
      screen_.show();
      loop_(this);
      screen_.clear();
    }
  }

  [[nodiscard]] auto frameRate() const { return frameRate_; }

  [[nodiscard]] auto frameTime() const { return 1000.0F / frameRate(); }

  inline auto &screen() { return screen_; }

  static inline auto delay(DWORD milliseconds) { Sleep(milliseconds); }

private:
  Screen &screen_;
  AnimFunc setup_, loop_;
  float frameRate_;
};

#endif // VERY_BASIC_ASCII_GRAPHICS_API_INCLUDE_ANIMATION_ANIMATION_ENGINE_HPP
