#include "geometry/transform.hpp"

#include "geometry/object.hpp"
#include "graphics/camera.hpp"

Transform::Transform(Object &object) : object_{object} {}

Transform::Transform(const M4F &matrix, Object &object)
    : transform_{matrix}, object_{object} {}

float Transform::operator[](size_t index) const {
  return transform_.data[index];
}

float Transform::operator()(size_t row, size_t col) const {
  return transform_(row, col);
}

V3F Transform::operator*(const V3F &vector) const {
  Matrix<float, 4, 1> extended{vector.x, vector.y, vector.z, 1};
  auto result{transform_ * extended};
  auto den{result.data[3]};
  if (isZero(den))
    den = 1.0f;
  return V3F{result[0], result[1], result[2]} / den;
}

void Transform::scale(V3F scales) {
  // cant scale a camera bucko
  if (dynamic_cast<Camera *>(&object_))
    return;
  transform_[0] *= scales.x;
  transform_[5] *= scales.y;
  transform_[10] *= scales.z;
  for (const auto &child : object_.children())
    child->transform().scale(scales);
}

void Transform::scale(float xs, float ys, float zs) { scale({xs, ys, zs}); }

void Transform::rotate(V3F eulerAngles) {
  // TODO: always rotate in place, but change axes
  using std::sin, std::cos;
  auto &a{eulerAngles.x}, b{eulerAngles.y}, c{eulerAngles.z};
  const float r00{cos(b) * cos(c)};
  const float r01{sin(a) * sin(b) * cos(c) - cos(a) * sin(c)};
  const float r02{cos(a) * sin(b) * cos(c) + sin(a) * sin(c)};
  const float r10{cos(b) * sin(c)};
  const float r11{sin(a) * sin(b) * sin(c) + cos(a) * cos(c)};
  const float r12{cos(a) * sin(b) * sin(c) - sin(a) * cos(c)};
  const float r20{-sin(b)};
  const float r21{sin(a) * cos(b)};
  const float r22{cos(a) * cos(b)};
  M4F rotationMatrix{
      r00, r01, r02, 0, //
      r10, r11, r12, 0, //
      r20, r21, r22, 0, //
      0,   0,   0,   1,
  };
  transform_ = rotationMatrix * transform_;
  if (auto cam = dynamic_cast<Camera *>(&object_))
    cam->updateWTC_();
  for (const auto &child : object_.children())
    child->transform().rotate(eulerAngles);
}

void Transform::rotate(float xr, float yr, float zr) { rotate({xr, yr, zr}); }

void Transform::rotateInPlace(V3F eulerAngles) {
  auto tmp{translation()};
  translate(-tmp);
  rotate(eulerAngles);
  translate(tmp);
}

void Transform::rotateInPlace(float xr, float yr, float zr) {
  rotateInPlace({xr, yr, zr});
}

void Transform::translate(V3F translation) {
  transform_[3] += translation.x;
  transform_[7] += translation.y;
  transform_[11] += translation.z;
  if (auto cam = dynamic_cast<Camera *>(&object_))
    cam->updateWTC_();
  for (const auto &child : object_.children())
    child->transform().translate(translation);
}

void Transform::translate(float xt, float yt, float zt) {
  translate({xt, yt, zt});
}

V3F Transform::forward() const {
  V3F col3{transform_(0, 2), transform_(1, 2), transform_(2, 2)};
  return col3.normalized();
}

V3F Transform::right() const {
  const V3F col1{transform_(0, 0), transform_(1, 0), transform_(2, 0)};
  return col1.normalized();
}

V3F Transform::up() const {
  V3F col2{transform_(0, 1), transform_(1, 1), transform_(2, 1)};
  return col2.normalized();
}

V3F Transform::translation() const { return {x(), y(), z()}; }

M4F Transform::operator*(const Transform &other) const {
  return transform_ * other.transform_;
}

float Transform::x() const { return transform_[3]; }

float Transform::y() const { return transform_[7]; }

float Transform::z() const { return transform_[11]; }

M4F operator*(const M4F &matrix, const Transform &transform) {
  return matrix * transform.transform_;
}