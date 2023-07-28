#include "animation/animation_engine.hpp"

#include <cmath>
#include <limits>

AnimationEngine::AnimationEngine(Screen &screen, AnimFunc setup, AnimFunc loop,
                                 float frameRate)
    : screen_{screen}, setup_{std::move(setup)}, loop_{std::move(loop)},
      frameRate_{frameRate} {}

void AnimationEngine::drawLine(V3F a, V3F b, float thickness, char fill) {
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
      // vertical lines were not drawn before, so I asked ChatGPT why and it
      // gave me this solution. I'm too lazy to check the underlying reason, but
      // it sure does work now. previously instead of the ternary op it was just
      // t = ((a + proj).x - a.x) / ab.x;
      t = std::abs(ab.x) < std::numeric_limits<float>::epsilon() * 10
              ? ap.y / ab.y
              : ((a + proj).x - a.x) / ab.x;
      if ((ap - proj).length() < thickness && 0.0F < t && t < 1.0F)
        screen_.set(i, j, fill);
    }
  }
}

void AnimationEngine::drawGraph(GV3F g, float thickness, char fill) {
  for (auto &vertex : g.vertices())
    vertex.transformAsPoint(g.transform());

  // TODO: eventually create a Camera class that has all these
  static constexpr auto fov{3.0f},
      fovRad{fov * (std::numbers::pi_v<float> / 180.0f)};
  static constexpr auto nearZ{0.0f},
      farZ{std::numeric_limits<float>::infinity()};
  const auto xFactor{1.0f / std::tan(fovRad / 2.0f)},
      yFactor{1.0f / std::tan(fovRad / 2.0f)};

  // TODO: tidy this up a bit
  for (auto &vertex : g.vertices()) {

    // perspective projection matrix stolen and adapted from
    // https://ogldev.org/www/tutorial12/tutorial12.html
    M4F perspectiveMatrix{xFactor,
                          0,
                          0,
                          0,
                          0,
                          yFactor,
                          0,
                          0,
                          0,
                          0,
                          (-nearZ - farZ) / (nearZ - farZ),
                          (2 * farZ * nearZ) / (nearZ - farZ),
                          0,
                          0,
                          -1,
                          1};
    vertex.transformAsPoint(perspectiveMatrix);
  }

  for (size_t i{}; i < g.order(); ++i)
    for (auto elem : g.edges(i))
      drawLine(g.vertices()[i], g.vertices()[elem], thickness, fill);
}

[[noreturn]] void AnimationEngine::run() {
  screen_.clear();
  setup_(this);
  for (;;) {
    screen_.show();
    loop_(this);
    screen_.clear();
  }
}

[[nodiscard]] inline float AnimationEngine::frameRate() const {
  return frameRate_;
}

[[nodiscard]] float AnimationEngine::frameTime() const {
  return 1000.0F / frameRate();
}

Screen &AnimationEngine::screen() { return screen_; }

void AnimationEngine::delay(DWORD milliseconds) { Sleep(milliseconds); }
