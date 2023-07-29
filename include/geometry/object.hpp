#ifndef VERY_BASIC_ASCII_GRAPHICS_API_INCLUDE_GEOMETRY_OBJECT_HPP
#define VERY_BASIC_ASCII_GRAPHICS_API_INCLUDE_GEOMETRY_OBJECT_HPP

#include "math/matrix.hpp"
#include "math/vector.hpp"

class Object {
public:
  void scale(V3F scales) {
    // TODO: try scaling to see if it works
    // M4F scalingMatrix{
    //    scales.x, 0,        0,        0, //
    //    0,        scales.y, 0,        0, //
    //    0,        0,        scales.z, 0, //
    //    0,        0,        0,        1,
    //};
    transform_.data[0] *= scales.x;
    transform_.data[5] *= scales.y;
    transform_.data[10] *= scales.z;
  }

  [[maybe_unused]] void scale(float x, float y, float z) { scale({x, y, z}); }

  void rotate(V3F eulerAngles) {
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

  void rotate(float x, float y, float z) { rotate({x, y, z}); }

  // these are pure virtual because without knowing the type of object we're
  // dealing with, there is no way to find out what its center is, and that
  // center is needed for the in-place rotation
  virtual void rotateInPlace(V3F) = 0;
  virtual void rotateInPlace(float, float, float) = 0;

  void translate(V3F translation) {
    transform_.data[3] += translation.x;
    transform_.data[7] += translation.y;
    transform_.data[11] += translation.z;
  }

  void translate(float x, float y, float z) { translate({x, y, z}); }

  const M4F &transform() { return transform_; }

  [[nodiscard]] V3F forward() const {
    V3F col3{transform_(0, 2), transform_(1, 2), transform_(2, 2)};
    auto sz{col3.length()};
    col3 /= sz;
    return col3;
  }

  [[nodiscard]] V3F right() const {
    const V3F col1{transform_(0, 0), transform_(1, 0), transform_(2, 0)};
    return col1.versor();
  }

  [[nodiscard]] V3F up() const {
    V3F col2{transform_(0, 1), transform_(1, 1), transform_(2, 1)};
    return col2.versor();
  }

protected:
  M4F transform_{
      1, 0, 0, 0, //
      0, 1, 0, 0, //
      0, 0, 1, 0, //
      0, 0, 0, 1,
  };

private:
};

#endif // VERY_BASIC_ASCII_GRAPHICS_API_INCLUDE_GEOMETRY_OBJECT_HPP
