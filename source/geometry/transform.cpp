#include "geometry/transform.hpp"

Transform::Transform(const M4F &matrix) : transform_{matrix} {}

float Transform::operator[](size_t index) const {
  return transform_.data[index];
}

float &Transform::operator[](size_t index) { return transform_.data[index]; }

float Transform::operator()(size_t row, size_t col) const {
  return transform_(row, col);
}

float &Transform::operator()(size_t row, size_t col) {
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

V3F Transform::forward() const {
  V3F col3{transform_(0, 2), transform_(1, 2), transform_(2, 2)};
  return col3.versor();
}

V3F Transform::right() const {
  const V3F col1{transform_(0, 0), transform_(1, 0), transform_(2, 0)};
  return col1.versor();
}

V3F Transform::up() const {
  V3F col2{transform_(0, 1), transform_(1, 1), transform_(2, 1)};
  return col2.versor();
}

void Transform::scale(V3F scales) {
  transform_[0] *= scales.x;
  transform_[5] *= scales.y;
  transform_[10] *= scales.z;
}

void Transform::scale(float xs, float ys, float zs) { scale({xs, ys, zs}); }

void Transform::rotate(V3F eulerAngles) {
  using std::sin, std::cos;
  auto &a{eulerAngles.x}, b{eulerAngles.y}, c{eulerAngles.z};
  M4F rotationMatrix{
      cos(b) * cos(c),
      sin(a) * sin(b) * cos(c) - cos(a) * sin(c),
      cos(a) * sin(b) * cos(c) + sin(a) * sin(c),
      0,
      cos(b) * sin(c),
      sin(a) * sin(b) * sin(c) + cos(a) * cos(c),
      cos(a) * sin(b) * sin(c) - sin(a) * cos(c),
      0,
      -sin(b),
      sin(a) * cos(b),
      cos(a) * cos(b),
      0,
      0,
      0,
      0,
      1,
  };
  transform_ = rotationMatrix * transform_;
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
}

void Transform::translate(float xt, float yt, float zt) {
  translate({xt, yt, zt});
}

V3F Transform::translation() const { return {x, y, z}; }

M4F Transform::operator*(const Transform &other) const {
  return transform_ * other.transform_;
}
