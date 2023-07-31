#ifndef VERY_BASIC_ASCII_GRAPHICS_API_INCLUDE_GRAPHICS_CAMERA_HPP
#define VERY_BASIC_ASCII_GRAPHICS_API_INCLUDE_GRAPHICS_CAMERA_HPP

#include "geometry/object.hpp"
#include "math/matrix.hpp"
#include "math/vector.hpp"

class Camera : public Object {
private:
  struct TooManyCameras : public std::exception {
    [[nodiscard]] const char *what() const noexcept override {
      return "Too many cameras.";
    }
  };

public:
  Camera(const std::string& name, float fovDeg, float aspectRatio)
      : Object(name), fovDeg_{fovDeg}, aspectRatio_{aspectRatio} {
    // this should only be a concern until we have Scenes implemented
    if (amtOfCameras_)
      throw TooManyCameras{};
    ++amtOfCameras_;
  }

  [[nodiscard]] const auto &perspective() const {
    static auto fovRad{fovDeg_ * (std::numbers::pi_v<float> / 180.0f)};
    static auto xFactor{aspectRatio_ / std::tan(fovRad / 2.0f)},
        yFactor{1.0f / std::tan(fovRad / 2.0f)};
    // perspective projection matrix stolen and adapted from
    // https://ogldev.org/www/tutorial12/tutorial12.html
    static M4F perspectiveMatrix{xFactor, 0,       0,  0, //
                                 0,       yFactor, 0,  0, //
                                 0,       0,       1,  0, //
                                 0,       0,       -1, 1};
    return perspectiveMatrix;
  }

  [[nodiscard]] auto worldToCamera() const {
    // as long as no deranged maniac tries to scale a camera, this should work.
    // also I did NOT want to recalculate the WTC matrix every time this is
    // called; ideally it'd be changed only when translating/rotating the
    // camera, but these methods are in the transform, so I can't really
    // override them here now can I? fuck

    // clang-format off
    return M4F{
        transform_[0], transform_[4], transform_[8], -(transform_[3]*transform_[0] + transform_[7]*transform_[4] + transform_[11]*transform_[8]),
        transform_[1], transform_[5], transform_[9], -(transform_[3]*transform_[1] + transform_[7]*transform_[5] + transform_[11]*transform_[9]),
        transform_[2], transform_[6], transform_[10], -(transform_[3]*transform_[2] + transform_[7]*transform_[6] + transform_[11]*transform_[10]),
        0, 0, 0, 1
    };
    // clang-format on
  }

private:
  float fovDeg_, aspectRatio_;
  static inline size_t amtOfCameras_{};
};

#endif // VERY_BASIC_ASCII_GRAPHICS_API_INCLUDE_GRAPHICS_CAMERA_HPP
