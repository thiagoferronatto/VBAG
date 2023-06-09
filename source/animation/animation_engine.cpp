#include "animation/animation_engine.hpp"

#include "graphics/ray.hpp"

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
      t = ((a + proj).x - a.x) / ab.x;
      if ((ap - proj).length() < thickness && 0.0F <= t && t <= 1.0F)
        screen_.set(i, j, fill);
    }
  }
}

void AnimationEngine::drawGraph(GV3F g, float tx, float ty, float tz,
                                float thickness, char fill) {
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

void AnimationEngine::drawMesh(const Mesh &mesh, float tx, float ty, float tz) {
  for (size_t i{}; i < screen_.height(); ++i) {
    for (size_t j{}; j < screen_.width(); ++j) {
      V3F p{float(j) - float(screen_.width()) / 2.0F,
            float(i) - float(screen_.height()) / 2.0F, 0.0F};
      R3F ray{p, {0, 0, -1}};
      auto [hit, point, normal]{ray.intersection(mesh)};
      if (hit)
        screen_.set(i, j, '#');
      // TODO: apply lighting and add pixel to framebuffer
    }
  }
}
