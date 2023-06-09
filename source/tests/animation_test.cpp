#include "tests/animation_test.hpp"

#include "animation/animation_engine.hpp"
#include "geometry/graph.hpp"
#include "geometry/mesh.hpp"
#include "output/screen.hpp"

// this is actually insane idk what it's doink
// #include <random>

void testAnimations() {
  //// rng stuff for making the movement less predictable (DONT USE IT, IT'S
  /// NUTS) / std::default_random_engine randEngine; /
  /// std::uniform_real_distribution<float> fltDist{-0.001F, 0.001F};

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
    // these are really not necessary, they just let me know what I'm doing l8tr
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

    // step 2: connecting those fuckers to make a nice wireframe
    g.addEdge(a, b);
    g.addEdge(a, c);
    g.addEdge(b, d);
    g.addEdge(c, d);
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

  // Mesh m;
  // m.addVertex({-5, 0, -1});
  // m.addVertex({5, 0, -1});
  // m.addVertex({0, 5, -1});
  // m.addVertex({10, 5, -1});
  // m.addVertex({5, 10, -1});
  //
  // m.addNormal({0, 0, 1});
  // m.addNormal({0, 0, 1});
  // m.addNormal({0, 0, 1});
  // m.addNormal({0, 0, 1});
  // m.addNormal({0, 0, 1});
  //
  // m.addTriangle({0, 1, 2});
  // m.addTriangle({2, 3, 4});

  // for (auto &vertex : m.vertices()) {
  //   printf("v %f, %f, %f\n", vertex.x - 20.F, vertex.y - 20.F, vertex.z);
  // }
  //
  // for (auto &normal : m.normals()) {
  //   printf("vn %f, %f, %f\n", normal.x, normal.y, normal.z);
  // }
  //
  // for (auto &triangle : m.triangles()) {
  //   auto v1{m.vertices()[triangle.first]}, v2{m.vertices()[triangle.second]},
  //       v3{m.vertices()[triangle.third]};
  //   printf("f %llu (%.1f, %.1f, %.1f), %llu (%.1f, %.1f, %.1f), %llu (%.1f, "
  //          "%.1f, %.1f)\n",
  //          triangle.first, v1.x, v1.y, v1.z, triangle.second, v2.x, v2.y,
  //          v2.z, triangle.third, v3.x, v3.y, v3.z);
  // }

  // auto setupFunc = [&](AnimationEngine *) {
  //   //
  // };
  //
  // auto loopFunc = [&](AnimationEngine *engine) {
  //   engine->screen().fill(' ');
  //   engine->drawMesh(m, 0, 0, 0);
  //   engine->drawLine({-40, 0, -1}, {40, 0, -1}, .5, '%');
  //   engine->drawLine({-40, 5, -1}, {40, 5, -1}, .5, '%');
  //   engine->drawLine({-40, 10, -1}, {40, 10, -1}, .5, '%');
  //   engine->drawLine({-40, -5, -1}, {40, -5, -1}, .5, '%');
  //   engine->drawLine({-40, -10, -1}, {40, -10, -1}, .5, '%');
  //   engine->drawLine({-0.01, -20, -1}, {0.01, 20, -1}, .5, '%');
  //   engine->drawLine({-5.01, -20, -1}, {-4.99, 20, -1}, .5, '%');
  //   engine->drawLine({4.99, -20, -1}, {5.01, 20, -1}, .5, '%');
  //   AnimationEngine::delay(500);
  // };

  Screen screen{90, 40};
  AnimationEngine engine{screen, setupFunc, loopFunc, 75};
  engine.run();
}