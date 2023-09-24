#ifndef VERY_BASIC_ASCII_GRAPHICS_API_INCLUDE_ANIMATION_ANIMATION_ENGINE_HPP
#define VERY_BASIC_ASCII_GRAPHICS_API_INCLUDE_ANIMATION_ANIMATION_ENGINE_HPP

#include <cmath>
#include <functional>
#include <utility>
#include <windows.h>

#include "geometry/graph.hpp"
#include "geometry/scene.hpp"
#include "graphics/camera.hpp"
#include "graphics/quad_mesh.hpp"
#include "graphics/triangle_mesh.hpp"
#include "output/d3d9_screen.hpp"

namespace vbag {

/// @class AnimationEngine
/// @brief The AnimationEngine class provides a framework for creating and
/// animating scenes.
///
/// The AnimationEngine class is responsible for managing scenes, drawing
/// elements, and controlling the animation loop. It should allow dynamic scene
/// changes, and provides functionality for drawing lines and graphs on the
/// screen.
class AnimationEngine {
  // TODO: add a setScene method, allowing for dynamic scene changing
public:
  /// @brief Alias for a function pointer used in setup and loop animations.
  ///
  /// An RenderFunc represents a function pointer that takes an AnimationEngine*
  /// as an argument. It is used to define setup and loop animations for the
  /// AnimationEngine.
  using RenderFunc = std::function<void(AnimationEngine *)>;

  /// @brief Constructs an AnimationEngine object with the given parameters.
  ///
  /// @param screen The reference to the Screen object where the animation will
  /// be displayed.
  /// @param setup The function to be executed once at the beginning of the
  /// animation.
  /// @param loop The function to be executed repeatedly during the animation
  /// loop.
  /// @param scene The initial scene to be displayed.
  /// @param frameRate The desired frame rate for the animation (default
  /// is 60.0).
  AnimationEngine(Screen &screen, RenderFunc setup, RenderFunc loop,
                  Scene scene, float frameRate = 60.0F);

  /// @brief Draws a graph on the screen.
  ///
  /// @param g A pointer to the object representing the graph.
  /// @param thickness The thickness of the graph (default is 3).
  /// @param fill The character representing the graph (default is '#').
  void drawGraph(const GV3F *g, float thickness = 3, char fill = '#');

  void drawMesh(const TriangleMesh *mesh);

  void drawQuadMesh(const QuadMesh *mesh);

  /// @brief Draws the current scene on the screen.
  ///
  /// @param thickness The default thickness for drawing elements (default is
  /// 3).
  /// @param fill The default character for drawing elements (default is '#').
  void draw(float = 3, char = '#');

  /// @brief Starts the animation loop and continues indefinitely until the
  /// program terminates.
  ///
  /// This function will continuously execute the setup animation function once
  /// and the loop animation function at the specified frame rate until the
  /// program is terminated or an exception is thrown.
  ///
  /// @note This function does not return.
  [[noreturn]] void run();

  /// @brief Returns the desired frame rate for the animation.
  ///
  /// @return The frame rate in frames per second.
  [[nodiscard]] float frameRate() const;

  /// @brief Returns the ideal time to render a single frame (frame time).
  ///
  /// @return The frame time in seconds.
  [[nodiscard]] float frameTime() const;

  /// @brief Returns a reference to the Screen object associated with the
  /// AnimationEngine.
  ///
  /// @return Reference to the Screen object.
  [[nodiscard]] Screen &screen();

  /// @brief Returns a reference to the Camera object associated with the
  /// AnimationEngine.
  ///
  /// @return Reference to the Camera object.
  [[nodiscard]] Camera &camera();

  /// @brief Static function to introduce a delay in the animation.
  ///
  /// @param milliseconds The number of milliseconds to delay the animation by.
  static void delay(float);

  /// @brief Returns the time elapsed between the current and the previous
  /// animation frame.
  ///
  /// @return The time elapsed in seconds.
  [[nodiscard]] float deltaTime() const;

private:
  Screen &screen_;    ///< Reference to the Screen object used for rendering.
  Scene scene_;       ///< The current scene being displayed.
  RenderFunc setup_;  ///< The setup animation function.
  RenderFunc loop_;   ///< The loop animation function.
  float frameRate_;   ///< The desired frame rate for the animation.
  float deltaTime_{}; ///< The time elapsed between the current and previous
                      ///< animation frame.
};

} // namespace vbag

#endif // VERY_BASIC_ASCII_GRAPHICS_API_INCLUDE_ANIMATION_ANIMATION_ENGINE_HPP
