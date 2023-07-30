#ifndef VERY_BASIC_ASCII_GRAPHICS_API_INCLUDE_GEOMETRY_TRANSFORM_HPP
#define VERY_BASIC_ASCII_GRAPHICS_API_INCLUDE_GEOMETRY_TRANSFORM_HPP

#include "math/matrix.hpp"
#include "math/vector.hpp"
#include "util/floating_point_comparison.hpp"

class Transform {
public:
  Transform() = default;
  Transform(const M4F &);
  [[nodiscard]] float operator[](size_t) const;
  float &operator[](size_t);
  [[nodiscard]] float operator()(size_t, size_t) const;
  float &operator()(size_t, size_t);
  [[nodiscard]] V3F operator*(const V3F &) const;
  [[nodiscard]] M4F operator*(const Transform&) const;
  [[nodiscard]] V3F forward() const;
  [[nodiscard]] V3F right() const;
  [[nodiscard]] V3F up() const;
  void scale(V3F);
  void scale(float, float, float);
  void rotate(V3F);
  void rotate(float, float, float);
  void rotateInPlace(V3F);
  void rotateInPlace(float, float, float);
  void translate(V3F);
  void translate(float, float, float);
  [[nodiscard]] V3F translation() const;

  friend auto operator*(const M4F &matrix, const Transform &transform) {
    return Transform{matrix * transform.transform_};
  }

private:
  M4F transform_{1, 0, 0, 0, //
                 0, 1, 0, 0, //
                 0, 0, 1, 0, //
                 0, 0, 0, 1};

public: // this looks kinda sus but transform_ needed to be initialized first :/
  const float &x{transform_[3]};
  const float &y{transform_[7]};
  const float &z{transform_[11]};
};

#endif // VERY_BASIC_ASCII_GRAPHICS_API_INCLUDE_GEOMETRY_TRANSFORM_HPP
