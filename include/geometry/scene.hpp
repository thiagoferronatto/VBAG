#ifndef VERY_BASIC_ASCII_GRAPHICS_API_INCLUDE_GEOMETRY_SCENE_HPP
#define VERY_BASIC_ASCII_GRAPHICS_API_INCLUDE_GEOMETRY_SCENE_HPP

#include <map>

#include "graphics/camera.hpp"

class Scene {
public:
  void addObject(Object *);
  void removeObject(const std::string &);
  void setMainCamera(const std::string &);
  [[nodiscard]] Object *object(const std::string &);
  [[nodiscard]] const Camera *mainCamera() const;
  [[nodiscard]] Camera *mainCamera();

  [[nodiscard]] auto begin() const { return objects_.begin(); }
  [[nodiscard]] auto end() const { return objects_.end(); }

private:
  std::map<std::string, Object *> objects_;
  Camera *mainCamera_{};
};

#endif // VERY_BASIC_ASCII_GRAPHICS_API_INCLUDE_GEOMETRY_SCENE_HPP
