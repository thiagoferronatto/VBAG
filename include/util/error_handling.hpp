#ifndef VERY_BASIC_ASCII_GRAPHICS_API_INCLUDE_UTIL_ERROR_HANDLING_HPP
#define VERY_BASIC_ASCII_GRAPHICS_API_INCLUDE_UTIL_ERROR_HANDLING_HPP

#include <exception>

enum ErrorType {
  NamedObjectIsNotACamera,
  SceneHasNoMainCameraSelected,
  NullPointerToObject,
  ChildIsSameAsParent,
  ObjectWithSameNameAlreadyInScene,
  ChildHasSameNameAsParent,
};

static const char *errorMessages[]{
    "Named object is not a camera.",
    "Scene has no main camera selected.",
    "Null pointer to object.",
    "Child is same as parent.",
    "Object with same name already in scene.",
    "Child has same name as parent.",
};

template <ErrorType errorType> class RuntimeError : public std::exception {
public:
  [[nodiscard]] const char *what() const noexcept override {
    return errorMessages[errorType];
  }
};

#endif // VERY_BASIC_ASCII_GRAPHICS_API_INCLUDE_UTIL_ERROR_HANDLING_HPP
