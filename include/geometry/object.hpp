#ifndef VERY_BASIC_ASCII_GRAPHICS_API_INCLUDE_GEOMETRY_OBJECT_HPP
#define VERY_BASIC_ASCII_GRAPHICS_API_INCLUDE_GEOMETRY_OBJECT_HPP

#include <string>
#include <utility>
#include <vector>

#include "geometry/transform.hpp"
#include "util/error_handling.hpp"

namespace vbag {

/// @class Object
/// @brief The Object class represents a generic object with a name, transform,
/// and hierarchical relationships.
///
/// The Object class serves as a base class for other classes in the scene. It
/// provides basic functionalities for managing object properties like the name,
/// transform, and parent-child relationships in a hierarchical scene structure.
class Object {
public:
  /// @brief Constructs an Object with the given name.
  ///
  /// @param name The name of the object.
  explicit Object(std::string);

  /// @brief Virtual destructor to allow dynamic_casts to work correctly.
  virtual ~Object();

  /// @brief Returns a constant reference to the transform of the object.
  ///
  /// @return A constant reference to the transform.
  [[nodiscard]] const Transform &transform() const;

  /// @brief Returns a reference to the transform of the object.
  ///
  /// @return A reference to the transform.
  Transform &transform();

  /// @brief Returns a constant pointer to the parent of the object.
  ///
  /// @return A constant pointer to the parent object.
  [[nodiscard]] const Object *parent() const;

  /// @brief Returns a constant reference to the vector of children objects.
  ///
  /// @return A constant reference to the vector of children objects.
  [[nodiscard]] const std::vector<Object *> &children() const;

  /// @brief Returns the name of the object.
  ///
  /// @return The name of the object.
  [[nodiscard]] const std::string &name() const;

  /// @brief Adds a child object to the current object.
  ///
  /// @param newChild The pointer to the child object to be added.
  /// @throw NullPointerToObject if newChild is a null pointer.
  /// @throw ChildIsSameAsParent if newChild is the same as the current object.
  /// @throw ChildHasSameNameAsParent if newChild has the same name as the
  /// current object.
  void addChild(Object *);

  /// @brief Removes a child object from the current object's list of children.
  ///
  /// @param child The pointer to the child object to be removed.
  void removeChild(Object *);

protected:
  Transform transform_{this};      ///< The transform of the object.
  Object *parent_{};               ///< Pointer to the parent object.
  std::vector<Object *> children_; ///< Vector of pointers to child objects.
  std::string name_;               ///< The name of the object.
};

} // namespace vbag

#endif // VERY_BASIC_ASCII_GRAPHICS_API_INCLUDE_GEOMETRY_OBJECT_HPP
