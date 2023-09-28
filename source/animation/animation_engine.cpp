#include "animation/animation_engine.hpp"

#include <chrono>
#include <utility>

#include "graphics/light.hpp"
#include "graphics/triangle_mesh.hpp"
#include "util/math.hpp"

namespace vbag {

static auto operator*(const M4F &matrix, const V3F &vector) {
  Matrix<float, 4, 1> extended{vector.x, vector.y, vector.z, 1};
  auto result{matrix * extended};
  auto den{result.data[3]};
  if (isZero(den))
    den = 1.0f;
  return V3F{result.data[0], result.data[1], result.data[2]} / den;
}

Engine::Engine(Screen &screen, RenderFunc setup, RenderFunc loop, Scene scene,
               float frameRate)
    : screen_{screen}, scene_{std::move(scene)}, setup_{std::move(setup)},
      loop_{std::move(loop)}, frameRate_{frameRate} {}

void Engine::queueGraph(const GV3F *g, std::vector<Line> &dst) {
  auto mainCamera{scene_.mainCamera()};
  if (!mainCamera)
    throw RuntimeError<SceneHasNoMainCameraSelected>{};
  const auto mvp{mainCamera->perspective() * mainCamera->worldToCamera() *
                 g->transform()};
  for (size_t i{}; i < g->order(); ++i) {
    auto a = mvp * g->vertices()[i];
    for (auto elem : g->edges(i)) {
      auto aa{a}, b = mvp * g->vertices()[elem];
      aa.x *= float(screen_.width());
      aa.y *= float(screen_.height());
      b.x *= float(screen_.width());
      b.y *= float(screen_.height());

      aa.x += float(screen_.width()) / 2;
      aa.y = float(screen_.height()) / 2 - aa.y;
      b.x += float(screen_.width()) / 2;
      b.y = float(screen_.height()) / 2 - b.y;

      if (aa.z <= 0 && b.z <= 0) // for clipping. this is ridiculous
        dst.push_back({aa, b, g->color()});
    }
  }
}

void Engine::drawMesh(const TriangleMesh *mesh) {
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
    // TODO: actually learn shaders and let the GPU do this
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
    v1.x += float(screen_.width()) / 2;
    v1.y = float(screen_.height()) / 2 - v1.y;
    v2.x += float(screen_.width()) / 2;
    v2.y = float(screen_.height()) / 2 - v2.y;
    v3.x += float(screen_.width()) / 2;
    v3.y = float(screen_.height()) / 2 - v3.y;
    // when the y coords are flipped, the normal is also flipped, so we just
    // change the order in which we pass them ahead and we're good (could also
    // use a D3DRS_CULLMODE to change the backface culling method to CCW)
    if (v1.z <= 0 && v2.z <= 0 && v3.z <= 0) // for clipping. this is ridiculous
      screen_.drawTriangle(v3, v2, v1, c3, c2, c1);
  }
}

void Engine::drawQuadMesh(const QuadMesh *mesh) {
  auto triangleMesh{mesh->asTriangleMesh()};
  drawMesh(&triangleMesh);
}

void Engine::draw() {
  std::vector<Line> lines;
  for (auto &[_, object] : scene_) {
    // cameras and lights are not drawn, so checking them here is dumb
    if (auto graphPointer{dynamic_cast<GV3F *>(object)}) {
      queueGraph(graphPointer, lines);
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
  screen_.drawLines(lines.data(), lines.size());
}

void Engine::run() {
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

[[nodiscard]] inline float Engine::frameRate() const { return frameRate_; }

[[nodiscard]] float Engine::frameTime() const { return 1000.0F / frameRate(); }

Screen &Engine::screen() { return screen_; }

Camera &Engine::camera() { return *scene_.mainCamera(); }

void Engine::delay(float milliseconds) { Sleep(DWORD(milliseconds)); }

float Engine::deltaTime() const { return deltaTime_; }

} // namespace vbag