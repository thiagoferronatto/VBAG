#ifndef VERY_BASIC_ASCII_GRAPHICS_API_INCLUDE_GRAPHICS_CAMERA_HPP
#define VERY_BASIC_ASCII_GRAPHICS_API_INCLUDE_GRAPHICS_CAMERA_HPP

#include "geometry/object.hpp"
#include "math/matrix.hpp"
#include "math/vector.hpp"

namespace vbag {

/// @class Camera
/// @brief The Camera class represents a camera in a 3D scene.
///
/// The Camera class is derived from the Object class and adds camera-specific
/// properties and functionality, such as perspective projection and the
/// transformation from world space to camera space.
class Camera : public Object {
public:
  /// @brief Constructs a Camera object with the given name, field of view, and
  /// aspect ratio.
  ///
  /// @param name The name of the camera.
  /// @param fovDeg The field of view angle in degrees.
  /// @param pixelAspectRatio_ The aspect ratio of the camera's view
  /// (width/height).
  Camera(const std::string &name, float fovDeg, float pixelAspectRatio_);

  /// @brief Returns the perspective projection matrix of the camera.
  ///
  /// The perspective projection matrix is used to convert 3D points from camera
  /// space to clip space during rendering.
  ///
  /// @return A constant reference to the perspective projection matrix.
  [[nodiscard]] const M4F &perspective() const;

  /// @brief Returns the world-to-camera transformation matrix of the camera.
  ///
  /// The world-to-camera transformation matrix converts 3D points from world
  /// space to camera space.
  ///
  /// @return A constant reference to the world-to-camera transformation matrix.
  [[nodiscard]] const M4F &worldToCamera() const;

private:
  friend Transform;

  /// @brief Updates the world-to-camera transformation matrix based on the
  /// camera's Transform.
  void updateWTC_();

  float fovDeg_;      ///< The field of view angle in degrees.
  float aspectRatio_; ///< The aspect ratio of the camera's view (width/height).
  M4F wtc_;           ///< The world-to-camera transformation matrix.
};

} // namespace vbag

#endif // VERY_BASIC_ASCII_GRAPHICS_API_INCLUDE_GRAPHICS_CAMERA_HPP
