#include "tests/animation_test.hpp"

#include "animation/animation_engine.hpp"
#include "math/graph.hpp"
#include "output/screen.hpp"

// this is actually insane idk what it's doink
// #include <random>

void testAnimations() {
  // rng stuff for making the movement less predictable (DONT USE IT, IT'S NUTS)
  // std::default_random_engine randEngine;
  // std::uniform_real_distribution<float> fltDist{-0.001F, 0.001F};

  // current angular accelerations
  float txa{.01}, tya{.005}, tza{.01};
  // current angular velocities
  float txv{}, tyv{}, tzv{};
  // current angles of rotation
  float tx{std::numbers::pi_v<float> / 2.0F}, ty{}, tz{};
  // max angular velocity and acceleration (otherwise pc go boom)
  constexpr float maxVel{.075F} /*, maxAcc{.001F}*/;
  // a graph that will hold our shape
  GV3F g;

  auto setupFunc = [&](AnimationEngine *engine) {
    // these are really not necessary, they just let me know what I'm doing in
    // step 2
    constexpr size_t a = 0, b = 1, c = 2, d = 3, e = 4, f = 5, g_ = 6, h = 7,
                     i = 8;
    // step 1: adding some vertices
    g.addVertex({-10, -20, -10});
    g.addVertex({-10, -20, 10});
    g.addVertex({10, -20, -10});
    g.addVertex({10, -20, 10});
    g.addVertex({0, 0, 0});
    g.addVertex({-10, 20, -10});
    g.addVertex({-10, 20, 10});
    g.addVertex({10, 20, -10});
    g.addVertex({10, 20, 10});
    g.addEdge(a, b);
    g.addEdge(a, c);
    g.addEdge(b, d);
    g.addEdge(c, d);

    // step 2: connecting those fuckers to make a nice wireframe
    g.addEdge(a, e);
    g.addEdge(b, e);
    g.addEdge(c, e);
    g.addEdge(d, e);
    g.addEdge(e, f);
    g.addEdge(e, g_);
    g.addEdge(e, h);
    g.addEdge(e, i);
    g.addEdge(f, g_);
    g.addEdge(f, h);
    g.addEdge(g_, i);
    g.addEdge(h, i);
  };

  auto loopFunc = [&](AnimationEngine *engine) {
    // incrementing the angle by the angular velocity
    tx = tx + txv > 2 * std::numbers::pi_v<float> ? 0.0F : tx + txv;
    ty = ty + tyv > 2 * std::numbers::pi_v<float> ? 0.0F : ty + tyv;
    tz = tz + tzv > 2 * std::numbers::pi_v<float> ? 0.0F : tz + tzv;

    // incrementing the angular velocity by the angular acceleration
    txv = std::min(txv + txa, maxVel);
    tyv = std::min(tyv + tya, maxVel);
    tzv = std::min(tzv + tza, maxVel);

    // randomly adding something to the angular accel (SIKE)
    // txa = std::min(txa + fltDist(randEngine), maxAcc);
    // tya = std::min(tya + fltDist(randEngine), maxAcc);
    // tza = std::min(tza + fltDist(randEngine), maxAcc);

    // clearing the previous frame
    engine->screen().fill(' ');

    // writing our shape to the framebuffer
    engine->drawGraph(g, tx, ty, tz, .5);

    // without this the frame is redrawn too quickly
    AnimationEngine::delay(static_cast<DWORD>(engine->frameTime()));
  };

  Screen screen{90, 40};
  AnimationEngine engine{screen, setupFunc, loopFunc};
  engine.run();
}