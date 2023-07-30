#ifndef VERY_BASIC_ASCII_GRAPHICS_API_INCLUDE_GEOMETRY_OBJECT_HPP
#define VERY_BASIC_ASCII_GRAPHICS_API_INCLUDE_GEOMETRY_OBJECT_HPP

#include "geometry/transform.hpp"

class Object {
public:
  [[nodiscard]] const auto &transform() const { return transform_; }
  auto &transform() { return transform_; }

protected:
  Transform transform_;
};

#endif // VERY_BASIC_ASCII_GRAPHICS_API_INCLUDE_GEOMETRY_OBJECT_HPP
