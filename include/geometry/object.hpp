#ifndef VERY_BASIC_ASCII_GRAPHICS_API_INCLUDE_GEOMETRY_OBJECT_HPP
#define VERY_BASIC_ASCII_GRAPHICS_API_INCLUDE_GEOMETRY_OBJECT_HPP

#include <string>
#include <utility>
#include <vector>

#include "geometry/transform.hpp"

class Object {
public:
  explicit Object(std::string name) : name_{std::move(name)} {}

  [[nodiscard]] const auto &transform() const { return transform_; }
  auto &transform() { return transform_; }

  [[nodiscard]] const auto *parent() const { return parent_; }

  [[nodiscard]] const auto &children() const { return children_; }

  [[nodiscard]] const auto &name() const { return name_; }

  void addChild(Object *child) {
    if (child->parent_) {
      (void)std::remove(child->parent_->children_.begin(),
                        child->parent_->children_.end(), child);
    }
    child->parent_ = this;
    children_.push_back(child);
  }

protected:
  Transform transform_{this};
  Object *parent_{};
  std::vector<Object *> children_;
  std::string name_;
};

#endif // VERY_BASIC_ASCII_GRAPHICS_API_INCLUDE_GEOMETRY_OBJECT_HPP
