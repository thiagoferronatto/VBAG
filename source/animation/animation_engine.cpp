#include "animation/animation_engine.hpp"

#include <cmath>

#include "util/floating_point_comparison.hpp"

static auto operator*(const M4F &matrix, const V3F &vector) {
  Matrix<float, 4, 1> extended{vector.x, vector.y, vector.z, 1};
  auto result{matrix * extended};
  auto den{result.data[3]};
  if (isZero(den))
    den = 1.0f;
  return V3F{result.data[0], result.data[1], result.data[2]} / den;
}

AnimationEngine::AnimationEngine(Screen &screen, AnimFunc setup, AnimFunc loop,
                                 float frameRate)
    : screen_{screen},
      camera_{3, Screen::stretchFactor * float(screen.width()) /
                     float(screen.height())},
      setup_{std::move(setup)}, loop_{std::move(loop)}, frameRate_{frameRate} {}

void AnimationEngine::drawLine(V3F a, V3F b, float thickness, char fill) {
  // the camera does not have a near plane, which means that stuff behind it is
  // drawn upside down. here is where I would stop that from happening, but this
  // seems to be doing absolutely nothing. idk how to solve this honestly.
  if (a.z > 0 || isZero(a.z) || b.z > 0 || isZero(b.z))
    return;

  a.z = b.z = 0.0F;
  const auto ab = b - a;
  const auto abn = ab.versor();
  V3F p{}, ap, proj;
  float t;
  const auto centerX{float(screen_.width()) / 2.0F};
  const auto centerY{float(screen_.height()) / 2.0F};
  // TODO: add bounding box to reduce unnecessary computations
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
      t = isZero(ab.x) ? ap.y / ab.y : ((a + proj).x - a.x) / ab.x;
      if ((ap - proj).length() < thickness && 0.0F < t && t < 1.0F)
        screen_.set(i, j, fill);
    }
  }
}

void AnimationEngine::drawGraph(const GV3F &g, float thickness, char fill) {
  const auto mvp{camera_.perspective() * camera_.worldToCamera() *
                 g.transform()};
  V3F a, b;
  for (size_t i{}; i < g.order(); ++i) {
    a = mvp * g.vertices()[i];
    for (auto elem : g.edges(i)) {
      b = mvp * g.vertices()[elem];
      drawLine(a, b, thickness, fill);
    }
  }
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

Camera &AnimationEngine::camera() { return camera_; }

void AnimationEngine::delay(DWORD milliseconds) { Sleep(milliseconds); }
