#ifndef VERY_BASIC_ASCII_GRAPHICS_API_INCLUDE_GEOMETRY_SCENE_HPP
#define VERY_BASIC_ASCII_GRAPHICS_API_INCLUDE_GEOMETRY_SCENE_HPP

#include <unordered_map>

#include "graphics/camera.hpp"

namespace vbag {

/// @class Scene
/// @brief The Scene class represents a scene containing objects and a main
/// camera.
///
/// The Scene class is responsible for managing objects within the scene and
/// setting the main camera for viewing the scene. It allows adding and removing
/// objects, setting the main camera, and provides functions to access objects
/// and the main camera.
class Scene {
public:
  /// @brief Adds an object to the scene.
  ///
  /// @param object A pointer to the Object to be added to the scene.
  /// @throw NullPointerToObject if object is a null pointer.
  /// @throw ObjectWithSameNameAlreadyInScene if an object with the same name is
  /// already present in the scene.
  void addObject(Object *object);

  /// @brief Removes an object from the scene.
  ///
  /// @param name The name of the object to be removed from the scene.
  /// @throw std::out_of_range if the object with the specified name is not
  /// found in the scene.
  void removeObject(const std::string &name);

  /// @brief Sets the main camera for the scene.
  ///
  /// @param cameraName The name of the Camera object to be set as the main
  /// camera.
  /// @throw NamedObjectIsNotACamera if the object with the specified name is
  /// not a Camera.
  void setMainCamera(const std::string &cameraName);

  /// @brief Returns a pointer to the object with the specified name in the
  /// scene.
  ///
  /// @param name The name of the object to retrieve.
  /// @throw std::out_of_range if the object with the specified name is not
  /// found in the scene.
  /// @return A pointer to the object if found.
  [[nodiscard]] Object *object(const std::string &name);

  /// @brief Returns a constant pointer to the main camera in the scene.
  ///
  /// @return A constant pointer to the main camera.
  [[nodiscard]] const Camera *mainCamera() const;

  /// @brief Returns a pointer to the main camera in the scene.
  ///
  /// @return A pointer to the main camera.
  [[nodiscard]] Camera *mainCamera();

  /// @brief Returns an iterator to the beginning of the objects in the scene.
  ///
  /// @return An iterator to the beginning of the objects.
  [[nodiscard]] auto begin() const { return objects_.begin(); }

  /// @brief Returns an iterator to the end of the objects in the scene.
  ///
  /// @return An iterator to the end of the objects.
  [[nodiscard]] auto end() const { return objects_.end(); }

private:
  std::unordered_map<std::string, Object *>
      objects_; ///< A map containing pointers to objects in the scene, indexed
                ///< by their names.
  Camera *mainCamera_{}; ///< Pointer to the main camera object in the scene.
};

} // namespace vbag

#endif // VERY_BASIC_ASCII_GRAPHICS_API_INCLUDE_GEOMETRY_SCENE_HPP
