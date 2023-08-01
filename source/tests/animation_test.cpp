#include "tests/animation_test.hpp"

#include "animation/animation_engine.hpp"
#include "geometry/graph.hpp"
#include "input/input.hpp"
#include "output/screen.hpp"

void testAnimations() {
  fputs("WASD to move the cube.\n"
        "Q and E to rotate the cube about the y axis.\n"
        "Spacebar to make the cube jump.\n"
        "IJKL to move the camera.\n"
        "U and O to rotate the camera about the y axis.\n"
        "N and M to rotate the camera about the x axis.\n"
        "Escape to close the application.\n"
        "Type a resolution to start (eg.: 100x25): ",
        stdout);
  size_t width, height;
  scanf_s("%zux%zu", &width, &height);

  V3F velocity{};

  Scene scene;

  // a graph that will hold our shape
  GV3F g{GV3F::cube("cube")}, h{GV3F::cube("lil_cube")},
      i{GV3F::cube("static_cube")};
  Camera camera{"main_camera", 3,
                Screen::stretchFactor * float(width) / float(height)};

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
    mainCam.transform().translate(0, 0, 10);
  };

  auto loopFunc = [&](AnimationEngine *engine) {
    // current accelerations
    V3F acceleration{};
    V3F rotation{};

    static constexpr auto factor{0.1f};
    static constexpr V3F gravity{0, -1, 0};

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

    acceleration = acceleration.normalized() * factor;

    // handling jump after accel normalization
    if (Input::getKey(KeyCode::Space) &&
        (cube.transform().y() < 0 || isZero(cube.transform().y())))
      acceleration += V3F::up();

    // moving the camera
    if (Input::getKey(KeyCode::I))
      mainCam.transform().translate(V3F::up() * factor);
    if (Input::getKey(KeyCode::J))
      mainCam.transform().translate(-V3F::right() * factor);
    if (Input::getKey(KeyCode::K))
      mainCam.transform().translate(-V3F::up() * factor);
    if (Input::getKey(KeyCode::L))
      mainCam.transform().translate(V3F::right() * factor);
    if (Input::getKey(KeyCode::U))
      mainCam.transform().rotateInPlace(V3F::up() * factor);
    if (Input::getKey(KeyCode::O))
      mainCam.transform().rotateInPlace(-V3F::up() * factor);
    if (Input::getKey(KeyCode::N))
      mainCam.transform().rotateInPlace(-mainCam.transform().right() * factor);
    if (Input::getKey(KeyCode::M))
      mainCam.transform().rotateInPlace(mainCam.transform().right() * factor);

    if (Input::getKey(KeyCode::Escape))
      exit(0);

    // incrementing the velocity by the acceleration
    velocity += acceleration;

    // applying movement and rotation
    cube.transform().translate(velocity);
    cube.transform().rotateInPlace(rotation);

    // simulating deceleration by drag
    if (cube.transform().y() < 0 || isZero(cube.transform().y())) {
      velocity.x *= 0.9f;
      velocity.z *= 0.9f;
    }

    // applying gravity and detecting collision with the ground plane y = 0
    if (cube.transform().y() < 0 || isZero(cube.transform().y()))
      cube.transform().translate(-V3F::up() * cube.transform().y());
    else
      velocity += gravity * factor;

    // clearing the previous frame
    engine->screen().fill(' ');

    engine->draw(.5);
  };

  Screen screen{width, height};
  AnimationEngine engine{screen, setupFunc, loopFunc, scene, 60};
  engine.run();
}