#include "tests/animation_test.hpp"

#include "animation/animation_engine.hpp"
#include "geometry/graph.hpp"
#include "graphics/light.hpp"
#include "graphics/quad_mesh.hpp"
#include "graphics/triangle_mesh.hpp"
#include "input/input.hpp"
#include "output/d3d9_screen.hpp"

#define WIREFRAME_GAME

void testAnimations(HINSTANCE instance) {
  vbag::Scene scene;

#ifdef WIREFRAME_GAME
  vbag::V3F velocity{};

  vbag::GV3F cube{vbag::GV3F::cube("cube")},
      lilCube{vbag::GV3F::cube("lil_cube")};

  static constexpr int64_t tileAmount{40};
  static constexpr float coveredArea{100},
      tileSize{coveredArea / float(tileAmount)};
  std::vector<vbag::GV3F> tiles;
  for (auto i{0ll}; i < tileAmount; ++i) {
    for (auto j{0ll}; j < tileAmount; ++j) {
      auto tile{vbag::GV3F::square(
          "tile" + std::to_string(i * tileAmount + j),
          vbag::RgbColor{float(i) / float(tileAmount),
                         1 - std::abs(float(i - j)) / float(tileAmount),
                         float(j) / float(tileAmount)})};
      tile.transform().scale(tileSize);
      tile.transform().translate(2 * tileSize * float(i - tileAmount / 2), 0,
                                 -2 * tileSize * float(j - tileAmount / 2));
      tiles.emplace_back(std::move(tile));
    }
  }

  vbag::Camera mainCam{"main_camera", 106, float(720) / float(1280)};

  cube.addChild(&lilCube);
  cube.addChild(&mainCam);
  scene.addObject(&cube); // automatically adds all of cube's children
  for (auto &staticCube : tiles)
    scene.addObject(&staticCube);

  scene.setMainCamera("main_camera");

  auto setupFunc = [&](vbag::Engine *engine) {
    lilCube.transform().scale(.5);
    lilCube.transform().translate(0, 1.5, 0);
    mainCam.transform().translate(2, 3, 10);
  };

  auto loopFunc = [&](vbag::Engine *engine) {
    vbag::V3F acceleration{};
    vbag::V3F rotation{};

    static constexpr auto factor{2.0f};
    static constexpr vbag::V3F gravity{0, -0.75, 0};

    if (vbag::Input::getKey(vbag::KeyCode::W) &&
        (cube.transform().y() <= 0 || vbag::isZero(cube.transform().y())))
      acceleration -= cube.transform().forward();
    if (vbag::Input::getKey(vbag::KeyCode::A) &&
        (cube.transform().y() <= 0 || vbag::isZero(cube.transform().y())))
      acceleration -= cube.transform().right();
    if (vbag::Input::getKey(vbag::KeyCode::S) &&
        (cube.transform().y() <= 0 || vbag::isZero(cube.transform().y())))
      acceleration += cube.transform().forward();
    if (vbag::Input::getKey(vbag::KeyCode::D) &&
        (cube.transform().y() <= 0 || vbag::isZero(cube.transform().y())))
      acceleration += cube.transform().right();
    if (vbag::Input::getKey(vbag::KeyCode::Q))
      rotation += vbag::V3F::up();
    if (vbag::Input::getKey(vbag::KeyCode::E))
      rotation -= vbag::V3F::up();

    acceleration = acceleration.normalized() * factor * engine->deltaTime();
    rotation *= engine->deltaTime();

    if (vbag::Input::getKey(vbag::KeyCode::Space) &&
        (cube.transform().y() < 0 || vbag::isZero(cube.transform().y())))
      acceleration += vbag::V3F::up() * 20 * engine->deltaTime();

    if (vbag::Input::getKey(vbag::KeyCode::I))
      mainCam.transform().translate(10 * vbag::V3F::up() * engine->deltaTime());
    if (vbag::Input::getKey(vbag::KeyCode::J))
      mainCam.transform().translate(-10 * mainCam.transform().right() *
                                    engine->deltaTime());
    if (vbag::Input::getKey(vbag::KeyCode::K))
      mainCam.transform().translate(-10 * vbag::V3F::up() *
                                    engine->deltaTime());
    if (vbag::Input::getKey(vbag::KeyCode::L))
      mainCam.transform().translate(10 * mainCam.transform().right() *
                                    engine->deltaTime());
    if (vbag::Input::getKey(vbag::KeyCode::U))
      mainCam.transform().rotateInPlace(vbag::V3F::up() * engine->deltaTime());
    if (vbag::Input::getKey(vbag::KeyCode::O))
      mainCam.transform().rotateInPlace(-vbag::V3F::up() * engine->deltaTime());
    if (vbag::Input::getKey(vbag::KeyCode::N))
      mainCam.transform().rotateInPlace(-mainCam.transform().right() *
                                        engine->deltaTime());
    if (vbag::Input::getKey(vbag::KeyCode::M))
      mainCam.transform().rotateInPlace(mainCam.transform().right() *
                                        engine->deltaTime());

    if (vbag::Input::getKey(vbag::KeyCode::Escape))
      exit(0);

    velocity += acceleration;

    cube.transform().translate(velocity);
    cube.transform().rotateInPlace(rotation);

    if (cube.transform().y() < 0 || vbag::isZero(cube.transform().y())) {
      velocity.x *= std::pow(0.1f, engine->deltaTime());
      velocity.z *= std::pow(0.1f, engine->deltaTime());
    }

    if (cube.transform().y() < 0 || vbag::isZero(cube.transform().y()))
      cube.transform().translate(-vbag::V3F::up() * cube.transform().y());
    else
      velocity += gravity * engine->deltaTime();
  };
#else
  vbag::QuadMesh mesh{"bah"};
  mesh.addVertex(-1, -1, 1);
  mesh.addVertex(1, -1, 1);
  mesh.addVertex(1, 1, 1);
  mesh.addVertex(-1, 1, 1);
  mesh.addVertex(-1, 1, -1);
  mesh.addVertex(-1, -1, -1);
  mesh.addVertex(1, -1, -1);
  mesh.addVertex(1, 1, -1);

  mesh.addNormal(-1, -1, 1);
  mesh.addNormal(1, -1, 1);
  mesh.addNormal(1, 1, 1);
  mesh.addNormal(-1, 1, 1);
  mesh.addNormal(-1, 1, -1);
  mesh.addNormal(-1, -1, -1);
  mesh.addNormal(1, -1, -1);
  mesh.addNormal(1, 1, -1);

  mesh.addQuad(0, 1, 2, 3);
  mesh.addQuad(1, 6, 7, 2);
  mesh.addQuad(6, 5, 4, 7);
  mesh.addQuad(5, 0, 3, 4);
  mesh.addQuad(5, 6, 1, 0);
  mesh.addQuad(3, 2, 7, 4);

  scene.addObject(&mesh);

  // vbag::PointLight light{"point_light"};
  // scene.addObject(&light);

  // TODO: figure out why the aspect ratio is squishing stuff
  vbag::Camera camera{"cam", 60, 720.0f / 1280};
  scene.addObject(&camera);
  scene.setMainCamera("cam");

  auto setupFunc = [&](vbag::Engine *engine) {
    camera.transform().translate(0, 0, 6);
  };

  auto loopFunc = [&](vbag::Engine *engine) {
    if (vbag::Input::getKey(vbag::KeyCode::W))
      camera.transform().translate(-5 * camera.transform().forward() *
                                   engine->deltaTime());
    if (vbag::Input::getKey(vbag::KeyCode::A))
      camera.transform().translate(-5 * camera.transform().right() *
                                   engine->deltaTime());
    if (vbag::Input::getKey(vbag::KeyCode::S))
      camera.transform().translate(5 * camera.transform().forward() *
                                   engine->deltaTime());
    if (vbag::Input::getKey(vbag::KeyCode::D))
      camera.transform().translate(5 * camera.transform().right() *
                                   engine->deltaTime());
    if (vbag::Input::getKey(vbag::KeyCode::Space))
      camera.transform().translate(5 * vbag::V3F::up() * engine->deltaTime());
    if (vbag::Input::getKey(vbag::KeyCode::LShift))
      camera.transform().translate(-5 * vbag::V3F::up() * engine->deltaTime());

    if (vbag::Input::getKey(vbag::KeyCode::Q))
      camera.transform().rotateInPlace(vbag::V3F::up() * engine->deltaTime());
    if (vbag::Input::getKey(vbag::KeyCode::E))
      camera.transform().rotateInPlace(-vbag::V3F::up() * engine->deltaTime());
    if (vbag::Input::getKey(vbag::KeyCode::R))
      camera.transform().rotateInPlace(camera.transform().right() *
                                       engine->deltaTime());
    if (vbag::Input::getKey(vbag::KeyCode::F))
      camera.transform().rotateInPlace(-camera.transform().right() *
                                       engine->deltaTime());

    if (vbag::Input::getKey(vbag::KeyCode::Escape))
      exit(0);
  };
#endif

  vbag::D3d9Screen screen{instance, "VBAG Demo", 1280, 720};
  vbag::Engine engine{screen, setupFunc, loopFunc, scene};
  engine.run();
}