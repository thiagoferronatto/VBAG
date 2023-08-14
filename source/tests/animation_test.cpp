#include "tests/animation_test.hpp"

#include "animation/animation_engine.hpp"
#include "geometry/graph.hpp"
#include "graphics/light.hpp"
#include "graphics/triangle_mesh.hpp"
#include "input/input.hpp"
#include "output/d3d9_screen.hpp"

void testAnimations(HINSTANCE instance) {

  Scene scene;

#if false
  V3F velocity{};

  // a graph that will hold our shape
  GV3F g{GV3F::cube("cube")}, h{GV3F::cube("lil_cube")},
      i{GV3F::cube("static_cube")};
  Camera camera{"main_camera", 0.25, float(800) / float(600)};

  g.addChild(&h);
  g.addChild(&camera);
  scene.addObject(&g); // automatically adds all of g's children
  scene.addObject(&i);
  scene.setMainCamera("main_camera");

  auto &cube{*scene.object("cube")};
  auto &lilCube{*scene.object("lil_cube")};
  auto &staticCube{*scene.object("static_cube")};
  auto &mainCam{*scene.mainCamera()};
  auto setupFunc = [&](AnimationEngine *engine) {
    staticCube.transform().scale(2, 2, 2);
    staticCube.transform().translate(0, 1, 0);
    lilCube.transform().scale(.5, .5, .5);
    lilCube.transform().translate(0, 1.5, 0);
    mainCam.transform().translate(2, 3, 10);
  };

  auto loopFunc = [&](AnimationEngine *engine) {
    // current accelerations
    V3F acceleration{};
    V3F rotation{};

    static constexpr auto factor{0.5f};
    static constexpr V3F gravity{0, -0.75, 0};

    // moving the cube
    if (Input::getKey(KeyCode::W) &&
        (cube.transform().y() <= 0 || isZero(cube.transform().y())))
      acceleration -= cube.transform().forward();
    if (Input::getKey(KeyCode::A) &&
        (cube.transform().y() <= 0 || isZero(cube.transform().y())))
      acceleration -= cube.transform().right();
    if (Input::getKey(KeyCode::S) &&
        (cube.transform().y() <= 0 || isZero(cube.transform().y())))
      acceleration += cube.transform().forward();
    if (Input::getKey(KeyCode::D) &&
        (cube.transform().y() <= 0 || isZero(cube.transform().y())))
      acceleration += cube.transform().right();
    if (Input::getKey(KeyCode::Q))
      rotation += V3F::up();
    if (Input::getKey(KeyCode::E))
      rotation -= V3F::up();

    acceleration = acceleration.normalized() * factor * engine->deltaTime();
    rotation *= engine->deltaTime();

    // handling jump after accel normalization
    if (Input::getKey(KeyCode::Space) &&
        (cube.transform().y() < 0 || isZero(cube.transform().y())))
      acceleration += V3F::up() * 20 * engine->deltaTime();

    // moving the camera
    if (Input::getKey(KeyCode::I))
      mainCam.transform().translate(V3F::up() * engine->deltaTime());
    if (Input::getKey(KeyCode::J))
      mainCam.transform().translate(-V3F::right() * engine->deltaTime());
    if (Input::getKey(KeyCode::K))
      mainCam.transform().translate(-V3F::up() * engine->deltaTime());
    if (Input::getKey(KeyCode::L))
      mainCam.transform().translate(V3F::right() * engine->deltaTime());
    if (Input::getKey(KeyCode::U))
      mainCam.transform().rotateInPlace(V3F::up() * engine->deltaTime());
    if (Input::getKey(KeyCode::O))
      mainCam.transform().rotateInPlace(-V3F::up() * engine->deltaTime());
    if (Input::getKey(KeyCode::N))
      mainCam.transform().rotateInPlace(-mainCam.transform().right() *
                                        engine->deltaTime());
    if (Input::getKey(KeyCode::M))
      mainCam.transform().rotateInPlace(mainCam.transform().right() *
                                        engine->deltaTime());

    if (Input::getKey(KeyCode::Escape))
      exit(0);

    // incrementing the velocity by the acceleration
    velocity += acceleration;

    // applying movement and rotation
    cube.transform().translate(velocity);
    cube.transform().rotateInPlace(rotation);

    // simulating deceleration by drag
    if (cube.transform().y() < 0 || isZero(cube.transform().y())) {
      velocity.x *= std::pow(0.1f, engine->deltaTime());
      velocity.z *= std::pow(0.1f, engine->deltaTime());
    }

    // applying gravity and detecting collision with the ground plane y = 0
    if (cube.transform().y() < 0 || isZero(cube.transform().y()))
      cube.transform().translate(-V3F::up() * cube.transform().y());
    else
      velocity += gravity * engine->deltaTime();
  };
#else
  TriangleMesh mesh{"bah"};
  mesh.addVertex(-1, -1, 1);
  mesh.addVertex(1, -1, 1);
  mesh.addVertex(1, 1, 1);
  mesh.addVertex(-1, 1, 1);
  mesh.addVertex(-1, 1, -1);
  mesh.addVertex(1, 1, -1);
  mesh.addVertex(1, -1, -1);
  mesh.addVertex(-1, -1, -1);

  mesh.addNormal(mesh.vertices()[0]);
  mesh.addNormal(mesh.vertices()[1]);
  mesh.addNormal(mesh.vertices()[2]);
  mesh.addNormal(mesh.vertices()[3]);
  mesh.addNormal(mesh.vertices()[4]);
  mesh.addNormal(mesh.vertices()[5]);
  mesh.addNormal(mesh.vertices()[6]);
  mesh.addNormal(mesh.vertices()[7]);

  mesh.addTriangle(0, 1, 2);
  mesh.addTriangle(0, 2, 3);
  mesh.addTriangle(1, 6, 5);
  mesh.addTriangle(1, 5, 2);
  mesh.addTriangle(6, 7, 4);
  mesh.addTriangle(6, 4, 5);
  mesh.addTriangle(7, 0, 3);
  mesh.addTriangle(7, 3, 4);
  mesh.addTriangle(7, 6, 1);
  mesh.addTriangle(7, 1, 0);
  mesh.addTriangle(3, 2, 5);
  mesh.addTriangle(3, 5, 4);

  scene.addObject(&mesh);

  PointLight light{"point_light"};
  scene.addObject(&light);

  // TODO: figure out why the aspect ratio is squishing stuff
  Camera camera{"cam", 60, 600.0f / 800};
  scene.addObject(&camera);
  scene.setMainCamera("cam");

  auto setupFunc = [&](AnimationEngine *engine) {
    camera.transform().translate(0, 0, 6);
    light.transform().translate(-10, 10, 10);
  };

  auto loopFunc = [&](AnimationEngine *engine) {
    if (Input::getKey(KeyCode::W))
      camera.transform().translate(-5 * camera.transform().forward() *
                                   engine->deltaTime());
    if (Input::getKey(KeyCode::A))
      camera.transform().translate(-5 * camera.transform().right() *
                                   engine->deltaTime());
    if (Input::getKey(KeyCode::S))
      camera.transform().translate(5 * camera.transform().forward() *
                                   engine->deltaTime());
    if (Input::getKey(KeyCode::D))
      camera.transform().translate(5 * camera.transform().right() *
                                   engine->deltaTime());
    if (Input::getKey(KeyCode::Space))
      camera.transform().translate(5 * V3F::up() * engine->deltaTime());
    if (Input::getKey(KeyCode::LShift))
      camera.transform().translate(-5 * V3F::up() * engine->deltaTime());

    if (Input::getKey(KeyCode::Q))
      camera.transform().rotateInPlace(V3F::up() * engine->deltaTime());
    if (Input::getKey(KeyCode::E))
      camera.transform().rotateInPlace(-V3F::up() * engine->deltaTime());
    if (Input::getKey(KeyCode::R))
      camera.transform().rotateInPlace(camera.transform().right() *
                                       engine->deltaTime());
    if (Input::getKey(KeyCode::F))
      camera.transform().rotateInPlace(-camera.transform().right() *
                                       engine->deltaTime());

    if (Input::getKey(KeyCode::Escape))
      exit(0);
  };
#endif

  D3D9Screen screen{instance, "VBAG Demo", 800, 600};
  AnimationEngine engine{screen, setupFunc, loopFunc, scene};
  engine.run();
}