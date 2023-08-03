#include "geometry/scene.hpp"
#include "geometry/graph.hpp"
#include "util/error_handling.hpp"

Scene::~Scene() {
  // for (auto &[_, objectPtr] : objects_)
  //   delete objectPtr;
}

void Scene::addObject(std::shared_ptr<Object> object) {
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
  auto cameraPointer{
      std::dynamic_pointer_cast<Camera>(objects_.at(cameraName))};
  if (!cameraPointer)
    throw RuntimeError<NamedObjectIsNotACamera>{};
  mainCamera_ = cameraPointer;
}

std::shared_ptr<Object> Scene::object(const std::string &name) {
  return objects_.at(name);
}

const std::shared_ptr<Camera> &Scene::mainCamera() const { return mainCamera_; }

std::shared_ptr<Camera> Scene::mainCamera() { return mainCamera_; }