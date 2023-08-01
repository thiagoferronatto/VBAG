#ifndef VERY_BASIC_ASCII_GRAPHICS_API_INCLUDE_GEOMETRY_OBJECT_HPP
#define VERY_BASIC_ASCII_GRAPHICS_API_INCLUDE_GEOMETRY_OBJECT_HPP

#include <string>
#include <utility>
#include <vector>

#include "geometry/transform.hpp"
#include "util/error_handling.hpp"

class Object {
public:
  explicit Object(std::string);

  // this is some bullshit idk why but if this aint here dynamic_casts dont work
  virtual ~Object();

  [[nodiscard]] const Transform &transform() const;
  Transform &transform();
  [[nodiscard]] const Object *parent() const;
  [[nodiscard]] const std::vector<Object *> &children() const;
  [[nodiscard]] const std::string &name() const;
  void addChild(Object *);
  void removeChild(Object *);

protected:
  Transform transform_{this};
  Object *parent_{};
  std::vector<Object *> children_;
  std::string name_;
};

#endif // VERY_BASIC_ASCII_GRAPHICS_API_INCLUDE_GEOMETRY_OBJECT_HPP
