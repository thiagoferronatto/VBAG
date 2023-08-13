#include "animation/animation_engine.hpp"

#include <chrono>
#include <utility>

#include "util/floating_point_comparison.hpp"

static auto operator*(const M4F &matrix, const V3F &vector) {
  Matrix<float, 4, 1> extended{vector.x, vector.y, vector.z, 1};
  auto result{matrix * extended};
  auto den{result.data[3]};
  if (isZero(den))
    den = 1.0f;
  return V3F{result.data[0], result.data[1], result.data[2]} / den;
}

AnimationEngine::AnimationEngine(Screen &screen, RenderFunc setup,
                                 RenderFunc loop, Scene scene, float frameRate)
    : screen_{screen}, scene_{std::move(scene)}, setup_{std::move(setup)},
      loop_{std::move(loop)}, frameRate_{frameRate} {}

void AnimationEngine::drawGraph(const GV3F *g, float, char fill) {
  auto mainCamera{scene_.mainCamera()};
  if (!mainCamera)
    throw RuntimeError<SceneHasNoMainCameraSelected>{};
  const auto mvp{mainCamera->perspective() * mainCamera->worldToCamera() *
                 g->transform()};
  V3F a, b;
  for (size_t i{}; i < g->order(); ++i) {
    a = mvp * g->vertices()[i];
    for (auto elem : g->edges(i)) {
      b = mvp * g->vertices()[elem];
      // I hate myself
      auto tmpA{a}, tmpB{b};
      tmpA.x += float(screen_.width()) / 2;
      tmpA.y = float(screen_.height()) / 2 - tmpA.y;
      tmpB.x += float(screen_.width()) / 2;
      tmpB.y = float(screen_.height()) / 2 - tmpB.y;
      screen_.drawLine(tmpA, tmpB);
    }
  }
}

void AnimationEngine::draw(float thickness, char fill) {
  for (auto &[_, object] : scene_) {
    auto cameraPointer{dynamic_cast<Camera *>(object)};
    if (cameraPointer)
      continue;
    auto graphPointer{dynamic_cast<GV3F *>(object)};
    // here's where other kinds of objects should be drawn
    if (graphPointer)
      drawGraph(graphPointer, thickness, fill);
  }
}

[[noreturn]] void AnimationEngine::run() {
  using namespace std::chrono;
  setup_(this);
  while (true) {
    auto start{steady_clock::now()};
    loop_(this);
    screen_.clear();
    draw();
    screen_.present();
    auto end{steady_clock::now()};
    auto elapsedMs{duration_cast<milliseconds>(end - start)};
    deltaTime_ = float(elapsedMs.count()) / 1e3f;
  }
}

[[nodiscard]] inline float AnimationEngine::frameRate() const {
  return frameRate_;
}

[[nodiscard]] float AnimationEngine::frameTime() const {
  return 1000.0F / frameRate();
}

Screen &AnimationEngine::screen() { return screen_; }

Camera &AnimationEngine::camera() { return *scene_.mainCamera(); }

void AnimationEngine::delay(float milliseconds) { Sleep(DWORD(milliseconds)); }

float AnimationEngine::deltaTime() const { return deltaTime_; }
