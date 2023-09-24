#ifndef VERY_BASIC_ASCII_GRAPHICS_API_INCLUDE_GRAPHICS_LIGHT_HPP
#define VERY_BASIC_ASCII_GRAPHICS_API_INCLUDE_GRAPHICS_LIGHT_HPP

#include "geometry/object.hpp"

namespace vbag {

class PointLight : public Object {
public:
  explicit PointLight(std::string name) : Object(std::move(name)) {}

  [[nodiscard]] auto intensity() const { return intensity_; }

private:
  float intensity_{1};
};

} // namespace vbag

#endif // VERY_BASIC_ASCII_GRAPHICS_API_INCLUDE_GRAPHICS_LIGHT_HPP
