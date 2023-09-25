#include "graphics/camera.hpp"
#include <numbers>

namespace vbag {

Camera::Camera(const std::string &name, float fovDeg, float pixelAspectRatio_)
    : Object(name), fovDeg_{fovDeg}, aspectRatio_{pixelAspectRatio_} {
  updateWTC_();
}

[[nodiscard]] const M4F &Camera::perspective() const {
  static auto fovRad{fovDeg_ * (std::numbers::pi_v<float> / 180.0f)};
  static auto xFactor{aspectRatio_ / std::tan(fovRad / 2.0f)},
      yFactor{1.0f / std::tan(fovRad / 2.0f)};
  // perspective projection matrix stolen
  // and adapted from https://ogldev.org/www/tutorial12/tutorial12.html
  // HACK: notice the -1e9 there. I have some idea why this prevents things that
  // are behind the camera from being drawn. keep it as high as possible.
  static M4F perspectiveMatrix{xFactor, 0,       0,  0,    //
                               0,       yFactor, 0,  0,    //
                               0,       0,       1,  -1e9, //
                               0,       0,       -1, 1};
  return perspectiveMatrix;
}

[[nodiscard]] const M4F &Camera::worldToCamera() const { return wtc_; }

void Camera::updateWTC_() {
  // clang-format off
    wtc_ = {
      transform_[0], transform_[4], transform_[8], -(transform_[3]*transform_[0] + transform_[7]*transform_[4] + transform_[11]*transform_[8]),
      transform_[1], transform_[5], transform_[9], -(transform_[3]*transform_[1] + transform_[7]*transform_[5] + transform_[11]*transform_[9]),
      transform_[2], transform_[6], transform_[10], -(transform_[3]*transform_[2] + transform_[7]*transform_[6] + transform_[11]*transform_[10]),
      0, 0, 0, 1
    };
  // clang-format on
}

} // namespace vbag