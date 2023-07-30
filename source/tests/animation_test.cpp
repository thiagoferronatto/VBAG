#include "tests/animation_test.hpp"

// WINDOWS ONLY
#undef USE_CONIO_H_INSTEAD_OF_WINDOWS_H
#ifdef USE_CONIO_H_INSTEAD_OF_WINDOWS_H
#include <conio.h>
#else
#include <windows.h>
#endif

#include "animation/animation_engine.hpp"
#include "geometry/graph.hpp"
#include "output/screen.hpp"

void testAnimations() {
  fputs("WASD movem o cubo (sim, eh um cubo so q alongado)\nQ e E "
       "rotacionam.\nEspaco faz ele pular.\nEsc fecha o game.\nDigite uma "
       "resolucao pra iniciar (ex.: 100x25): ", stdout);
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
    g.translate(0, 0, -5);
  };

  auto loopFunc = [&](AnimationEngine *engine) {
    // current accelerations
    V3F acceleration{};
    V3F rotation{};

    static constexpr auto accelerationFactorPerFrame{0.05f};
    static constexpr V3F gravity{0, 0.1f, 0};

    // getting some keyboard input (WINDOWS ONLY)
#ifdef USE_CONIO_H_INSTEAD_OF_WINDOWS_H
    if (_kbhit()) {
      auto key{_getch()};
      switch (key) {
      case 'w':
        acceleration -= g.forward() * accelerationFactorPerFrame;
        break;
      case 'a':
        acceleration -= g.right() * accelerationFactorPerFrame;
        break;
      case 's':
        acceleration += g.forward() * accelerationFactorPerFrame;
        break;
      case 'd':
        acceleration += g.right() * accelerationFactorPerFrame;
        break;
      case 'q':
        rotation += V3F{0, accelerationFactorPerFrame, 0};
        break;
      case 'e':
        rotation -= V3F{0, accelerationFactorPerFrame, 0};
        break;
      default:
        break;
      }
    }
#else  // the much prettier alternative
    if (GetAsyncKeyState('W') & 0x8000)
      acceleration -= g.forward();

    if (GetAsyncKeyState('A') & 0x8000)
      acceleration -= g.right();

    if (GetAsyncKeyState('S') & 0x8000)
      acceleration += g.forward();

    if (GetAsyncKeyState('D') & 0x8000)
      acceleration += g.right();

    if (GetAsyncKeyState('Q') & 0x8000)
      rotation += V3F{0, 1, 0};

    if (GetAsyncKeyState('E') & 0x8000)
      rotation -= V3F{0, 1, 0};

    acceleration = acceleration.versor() * accelerationFactorPerFrame;
    rotation *= accelerationFactorPerFrame;

    if ((GetAsyncKeyState(VK_SPACE) & 0x8000) && g.transform()(1, 3) <= 0)
      acceleration += {0, 1, 0};

    if (GetAsyncKeyState(VK_ESCAPE) & 0x8000)
      exit(0);
#endif // USE_CONIO_H_INSTEAD_OF_WINDOWS_H

    // incrementing the velocity by the acceleration
    velocity += acceleration;

    // applying movement and rotation
    g.translate(velocity);
    g.rotateInPlace(rotation);

    // simulating deceleration by drag
    velocity *= 0.9f;

    // applying gravity and detecting collision with the ground plane y = 0
    if (g.transform()(1, 3) < 0)
      g.translate(0, -g.transform()(1, 3), 0);
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