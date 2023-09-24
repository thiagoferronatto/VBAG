#include "geometry/scene.hpp"
#include "util/error_handling.hpp"

namespace vbag {

void Scene::addObject(Object *object) {
  if (!object)
    throw RuntimeError<NullPointerToObject>{};
  if (objects_.find(object->name()) != objects_.end())
    throw RuntimeError<ObjectWithSameNameAlreadyInScene>{};
  objects_.emplace(object->name(), object);
  for (auto child : object->children())
    addObject(child);
}

void Scene::removeObject(const std::string &name) {
  auto object{objects_.at(name)};
  for (auto child : object->children())
    removeObject(child->name());
  objects_.erase(name);
}

void Scene::setMainCamera(const std::string &cameraName) {
  auto cameraPointer{dynamic_cast<Camera *>(objects_.at(cameraName))};
  if (!cameraPointer)
    throw RuntimeError<NamedObjectIsNotACamera>{};
  mainCamera_ = cameraPointer;
}

Object *Scene::object(const std::string &name) { return objects_.at(name); }

const Camera *Scene::mainCamera() const { return mainCamera_; }

Camera *Scene::mainCamera() { return mainCamera_; }

} // namespace vbag