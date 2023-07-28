#include "tests/animation_test.hpp"

// WINDOWS ONLY
#include <conio.h>

#include "animation/animation_engine.hpp"
#include "geometry/graph.hpp"
#include "geometry/mesh.hpp"
#include "output/screen.hpp"

void testAnimations() {
  // current angular velocities
  float txv{}, tyv{}, tzv{};
  // current angles of rotation
  float tx{std::numbers::pi_v<float> / 2.0F}, ty{}, tz{};
  // max angular velocity and acceleration (otherwise pc go boom)
  constexpr float maxVel{.075F} /*, maxAcc{.001F}*/;
  // a graph that will hold our shape
  GV3F g;

  auto setupFunc = [&](AnimationEngine *engine) {
    // these are really not necessary, they just let me know what I'm doing l8tr
    constexpr size_t a = 0, b = 1, c = 2, d = 3, e = 4, f = 5, g_ = 6, h = 7;

    // a cube
    constexpr auto scale{10.0f};
    g.addVertex(V3F{-1, 1, 1} * scale);
    g.addVertex(V3F{-1, -1, 1} * scale);
    g.addVertex(V3F{1, -1, 1} * scale);
    g.addVertex(V3F{1, 1, 1} * scale);
    g.addVertex(V3F{1, 1, -1} * scale);
    g.addVertex(V3F{-1, 1, -1} * scale);
    g.addVertex(V3F{-1, -1, -1} * scale);
    g.addVertex(V3F{1, -1, -1} * scale);

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
  };

  // moving it away from the camera a lil bit
  g.translate(0, 0, -30);

  auto loopFunc = [&](AnimationEngine *engine) {
    // current angular accelerations
    float txa{}, tya{}, tza{};

    static constexpr float accelerationFactorPerFrame{0.1f};

    // getting some keyboard input (WINDOWS ONLY)
    if (_kbhit()) {
      auto key{_getch()};
      switch (key) {
      case 'w':
        tza = -accelerationFactorPerFrame;
        break;
      case 'a':
        txa = -accelerationFactorPerFrame;
        break;
      case 's':
        tza = accelerationFactorPerFrame;
        break;
      case 'd':
        txa = accelerationFactorPerFrame;
        break;
      case 'q':
        tya = -accelerationFactorPerFrame;
        break;
      case 'e':
        tya = accelerationFactorPerFrame;
        break;
      default:
        break;
      }
    }

    // incrementing the velocity by the angular acceleration
    txv += txa;
    tyv += tya;
    tzv += tza;

    // moving the cube by its current velocity
    g.translate(txv, tyv, tzv);

    // simulating deceleration by drag
    txv *= 0.95f;
    tyv *= 0.95f;
    tzv *= 0.95f;

    // clearing the previous frame
    engine->screen().fill(' ');

    // writing our shape to the framebuffer
    engine->drawGraph(g, .5);

    // without this the frame might be redrawn too quickly
    AnimationEngine::delay(static_cast<DWORD>(engine->frameTime()));
  };

  Screen screen{180, 40};
  AnimationEngine engine{screen, setupFunc, loopFunc, 75};
  engine.run();
}