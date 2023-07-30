#include "tests/animation_test.hpp"

#include <windows.h>

#include "animation/animation_engine.hpp"
#include "geometry/graph.hpp"
#include "output/screen.hpp"

void testAnimations() {
  fputs("WASD movem o cubo\n"
        "Q e E rotacionam o cubo.\n"
        "Espaco faz o cubo pular.\n"
        "IJKL movimentam a camera.\n"
        "U e O rotacionam a camera.\n"
        "Esc fecha o game.\n"
        "Digite uma resolucao pra iniciar (ex.: 100x25): ",
        stdout);
  size_t width, height;
  scanf_s("%zux%zu", &width, &height);

  V3F velocity{};

  // a graph that will hold our shape
  GV3F g;

  auto setupFunc = [&](AnimationEngine *engine) {
    // these are really not necessary, they just let me know what I'm doing l8tr
    constexpr size_t a = 0, b = 1, c = 2, d = 3, e = 4, f = 5, g_ = 6, h = 7;

    // a cube
    g.addVertex({-1, 1, 1});
    g.addVertex({-1, -1, 1});
    g.addVertex({1, -1, 1});
    g.addVertex({1, 1, 1});
    g.addVertex({1, 1, -1});
    g.addVertex({-1, 1, -1});
    g.addVertex({-1, -1, -1});
    g.addVertex({1, -1, -1});

    g.addEdge(a, b);
    g.addEdge(a, d);
    g.addEdge(a, f);
    g.addEdge(b, c);
    g.addEdge(b, g_);
    g.addEdge(c, d);
    g.addEdge(c, h);
    g.addEdge(d, e);
    g.addEdge(e, f);
    g.addEdge(e, h);
    g.addEdge(f, g_);
    g.addEdge(g_, h);

    // moving it away from the camera a lil bit
    g.transform().translate(0, 0, -5);

    // this should visually compensate for the axis-aligned spacing of the
    // grid of characters in the console
    // g.transform().scale(1, .5, 1);
  };

  auto loopFunc = [&](AnimationEngine *engine) {
    // current accelerations
    V3F acceleration{};
    V3F rotation{};

    static constexpr auto accelerationFactorPerFrame{0.05f};
    static constexpr V3F gravity{0, 0.1f, 0};

    // getting some keyboard input (WINDOWS ONLY)
    if (GetAsyncKeyState('W') & 0x8000)
      acceleration -= g.transform().forward();

    if (GetAsyncKeyState('A') & 0x8000)
      acceleration -= g.transform().right();

    if (GetAsyncKeyState('S') & 0x8000)
      acceleration += g.transform().forward();

    if (GetAsyncKeyState('D') & 0x8000)
      acceleration += g.transform().right();

    if (GetAsyncKeyState('Q') & 0x8000)
      rotation += V3F::up();

    if (GetAsyncKeyState('E') & 0x8000)
      rotation -= V3F::up();

    if (GetAsyncKeyState('I') & 0x8000)
      engine->camera().transform().translate(V3F::up() *
                                             accelerationFactorPerFrame);
    if (GetAsyncKeyState('J') & 0x8000)
      engine->camera().transform().translate(-V3F::right() *
                                             accelerationFactorPerFrame);
    if (GetAsyncKeyState('K') & 0x8000)
      engine->camera().transform().translate(-V3F::up() *
                                             accelerationFactorPerFrame);
    if (GetAsyncKeyState('L') & 0x8000)
      engine->camera().transform().translate(V3F::right() *
                                             accelerationFactorPerFrame);

    if (GetAsyncKeyState('U') & 0x8000)
      engine->camera().transform().rotateInPlace(V3F::up() *
                                                 accelerationFactorPerFrame);

    if (GetAsyncKeyState('O') & 0x8000)
      engine->camera().transform().rotateInPlace(-V3F::up() *
                                                 accelerationFactorPerFrame);

    acceleration = acceleration.versor() * accelerationFactorPerFrame;
    rotation *= accelerationFactorPerFrame;

    if ((GetAsyncKeyState(VK_SPACE) & 0x8000) && g.transform().y() <= 0)
      acceleration += V3F::up();

    if (GetAsyncKeyState(VK_ESCAPE) & 0x8000)
      exit(0);

    // incrementing the velocity by the acceleration
    velocity += acceleration;

    // applying movement and rotation
    g.transform().translate(velocity);
    g.transform().rotateInPlace(rotation);

    // simulating deceleration by drag
    velocity *= 0.9f;

    // applying gravity and detecting collision with the ground plane y = 0
    if (g.transform().y() < 0)
      g.transform().translate(0, -g.transform().y(), 0);
    else
      velocity -= gravity;

    // clearing the previous frame
    engine->screen().fill(' ');

    // writing our shape to the framebuffer
    engine->drawGraph(g, .5);

    // without this the frame might be redrawn too quickly
    AnimationEngine::delay(static_cast<DWORD>(engine->frameTime()));
  };

  Screen screen{width, height};
  AnimationEngine engine{screen, setupFunc, loopFunc, 60};
  engine.run();
}