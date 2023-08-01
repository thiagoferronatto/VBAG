#ifndef VERY_BASIC_ASCII_GRAPHICS_API_INCLUDE_GRAPHICS_CAMERA_HPP
#define VERY_BASIC_ASCII_GRAPHICS_API_INCLUDE_GRAPHICS_CAMERA_HPP

#include "geometry/object.hpp"
#include "math/matrix.hpp"
#include "math/vector.hpp"

class Camera : public Object {
public:
  Camera(const std::string &, float, float);

  [[nodiscard]] const M4F &perspective() const;
  [[nodiscard]] const M4F &worldToCamera() const;

private:
  friend Transform;
  
  void updateWTC_();

  float fovDeg_, aspectRatio_;
  M4F wtc_;
};

#endif // VERY_BASIC_ASCII_GRAPHICS_API_INCLUDE_GRAPHICS_CAMERA_HPP
