#include "geometry/object.hpp"

Object::Object(std::string name) : name_{std::move(name)} {}

Object::~Object() = default;

const Transform &Object::transform() const { return transform_; }

Transform &Object::transform() { return transform_; }

const std::shared_ptr<Object> &Object::parent() const { return parent_; }

const std::vector<std::shared_ptr<Object>> &Object::children() const {
  return children_;
}

const std::string &Object::name() const { return name_; }

void Object::addChild(std::shared_ptr<Object> newChild) {
  if (!newChild)
    throw RuntimeError<NullPointerToObject>{};
  if (newChild.get() == this)
    throw RuntimeError<ChildIsSameAsParent>{};
  if (newChild->name_ == name_)
    throw RuntimeError<ChildHasSameNameAsParent>{};
  if (newChild->parent_)
    newChild->parent_->removeChild(newChild);
  newChild->parent_ = std::shared_ptr<Object>{this};
  children_.push_back(newChild);
}

void Object::removeChild(std::shared_ptr<Object> child) {
  // this just compares pointers, could be dangerous
  (void)std::remove(children_.begin(), children_.end(), child);
  child->parent_ = nullptr;
}