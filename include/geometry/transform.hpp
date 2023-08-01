#ifndef VERY_BASIC_ASCII_GRAPHICS_API_INCLUDE_GEOMETRY_TRANSFORM_HPP
#define VERY_BASIC_ASCII_GRAPHICS_API_INCLUDE_GEOMETRY_TRANSFORM_HPP

#include "math/matrix.hpp"
#include "math/vector.hpp"
#include "util/floating_point_comparison.hpp"

class Object;

class Transform {
public:
  explicit Transform(Object *object) : object_{object} {}
  Transform(const M4F &, Object *);

  [[nodiscard]] float operator[](size_t) const;
  [[nodiscard]] float operator()(size_t, size_t) const;
  [[nodiscard]] V3F operator*(const V3F &) const;
  [[nodiscard]] M4F operator*(const Transform &) const;
  void scale(V3F);
  void scale(float, float, float);
  void rotate(V3F);
  void rotate(float, float, float);
  void rotateInPlace(V3F);
  void rotateInPlace(float, float, float);
  void translate(V3F);
  void translate(float, float, float);
  [[nodiscard]] V3F forward() const;
  [[nodiscard]] V3F right() const;
  [[nodiscard]] V3F up() const;
  [[nodiscard]] V3F translation() const;
  [[nodiscard]] float x() const { return transform_[3]; }
  [[nodiscard]] float y() const { return transform_[7]; }
  [[nodiscard]] float z() const { return transform_[11]; }

  [[nodiscard]] friend M4F operator*(const M4F &matrix,
                                     const Transform &transform) {
    return matrix * transform.transform_;
  }

private:
  M4F transform_{1, 0, 0, 0, //
                 0, 1, 0, 0, //
                 0, 0, 1, 0, //
                 0, 0, 0, 1};
  Object *object_;
};

#endif // VERY_BASIC_ASCII_GRAPHICS_API_INCLUDE_GEOMETRY_TRANSFORM_HPP
