#include "animation/animation_engine.hpp"

#include <chrono>
#include <utility>

#include "graphics/light.hpp"
#include "graphics/triangle_mesh.hpp"
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

void AnimationEngine::drawGraph(const GV3F *g, float, char) {
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
      a.x *= float(screen_.width());
      a.y *= float(screen_.height());
      b.x *= float(screen_.width());
      b.y *= float(screen_.height());
      // I hate myself
      auto tmpA{a}, tmpB{b};
      tmpA.x += float(screen_.width()) / 2;
      tmpA.y = float(screen_.height()) / 2 - tmpA.y;
      tmpB.x += float(screen_.width()) / 2;
      tmpB.y = float(screen_.height()) / 2 - tmpB.y;
      screen_.drawLine(tmpA, tmpB, D3DCOLOR_XRGB(255, 255, 255));
    }
  }
}

void AnimationEngine::drawMesh(const TriangleMesh *mesh) {
  auto mainCamera{scene_.mainCamera()};
  if (!mainCamera)
    throw RuntimeError<SceneHasNoMainCameraSelected>{};
  const auto mvp{mainCamera->perspective() * mainCamera->worldToCamera() *
                 mesh->transform()};
  // FIXME: something's wrong with the lighting
#if defined(ENABLE_LIGHTING)
  std::vector<D3DCOLOR> colors;
  for (size_t i{}; i < mesh->vertices().size(); ++i) {
    float finalIntensity{};
    for (auto &[_, object] : scene_) {
      auto lightPointer{dynamic_cast<PointLight *>(object)};
      if (!lightPointer)
        continue;
      auto lightPos{mvp * lightPointer->transform().translation()},
          vertexPos{mvp * mesh->vertices()[i]},
          vertexNormal{mvp.transposeOfInverse() * mesh->normals()[i]};
      auto dot{(vertexPos - lightPos).dot(vertexNormal)};
      finalIntensity += 255.0f * lightPointer->intensity() * fabsf(dot);
    }
    finalIntensity = fmin(255.0f, finalIntensity);
    auto intEnsity{int(finalIntensity)};
    colors.emplace_back(D3DCOLOR_XRGB(intEnsity, intEnsity, intEnsity));
  }
#endif
  for (auto triangle : mesh->triangles()) {
    auto v1{mvp * mesh->vertices()[triangle.v1]},
        v2{mvp * mesh->vertices()[triangle.v2]},
        v3{mvp * mesh->vertices()[triangle.v3]};
    // TODO: gotta do this bc the mvp maps the vertices to NDCs, but the
    //       rasterizer right now doesn't take that into account, so for the
    //       moment we're just scaling them back up. This is also somehow linked
    //       to the camera's aspect ratio.
    v1.x *= float(screen_.width());
    v1.y *= float(screen_.height());
    v2.x *= float(screen_.width());
    v2.y *= float(screen_.height());
    v3.x *= float(screen_.width());
    v3.y *= float(screen_.height());
#if defined(ENABLE_LIGHTING)
    auto c1{colors[triangle.v1]}, c2{colors[triangle.v2]},
        c3{colors[triangle.v3]};
#else
    auto c1{D3DCOLOR_XRGB(255, 0, 0)}, c2{D3DCOLOR_XRGB(0, 255, 0)},
        c3{D3DCOLOR_XRGB(0, 0, 255)};
#endif
    // TODO: this mapping to the center of the screen is also done due to the
    //       lack of NDCs being used by the d3d rasterization.
    v1.x += float(screen_.width()) / 2;
    v1.y = float(screen_.height()) / 2 - v1.y;
    v2.x += float(screen_.width()) / 2;
    v2.y = float(screen_.height()) / 2 - v2.y;
    v3.x += float(screen_.width()) / 2;
    v3.y = float(screen_.height()) / 2 - v3.y;
    // when the y coords are flipped, the normal is also flipped, so we just
    // change the order in which we pass them ahead and we're good (could also
    // use a D3DRS_CULLMODE to change the backface culling method to CCW)
    screen_.drawTriangle(v3, v2, v1, c1, c2, c3);
  }
}

void AnimationEngine::drawQuadMesh(const QuadMesh *mesh) {
  auto triangleMesh{mesh->asTriangleMesh()};
  drawMesh(&triangleMesh);
}

void AnimationEngine::draw(float thickness, char fill) {
  for (auto &[_, object] : scene_) {
    // cameras and lights are not drawn, so checking them here is dumb
    if (auto graphPointer{dynamic_cast<GV3F *>(object)}) {
      drawGraph(graphPointer, thickness, fill);
      continue;
    }
    if (auto triangleMeshPointer{dynamic_cast<TriangleMesh *>(object)}) {
      drawMesh(triangleMeshPointer);
      continue;
    }
    if (auto quadMeshPointer{dynamic_cast<QuadMesh *>(object)}) {
      drawQuadMesh(quadMeshPointer);
      continue;
    }
  }
}

void AnimationEngine::run() {
  auto renderThread{std::thread{[&]() {
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
  }}};

  MSG msg{};
  while (GetMessage(&msg, nullptr, 0, 0) > 0) {
    TranslateMessage(&msg);
    DispatchMessage(&msg);
  }

  renderThread.join();
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
