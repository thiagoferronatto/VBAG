#ifndef VERY_BASIC_ASCII_GRAPHICS_API_INCLUDE_UTIL_ERROR_HANDLING_HPP
#define VERY_BASIC_ASCII_GRAPHICS_API_INCLUDE_UTIL_ERROR_HANDLING_HPP

#include <exception>

/// @enum ErrorType
/// @brief Enumeration representing different types of runtime errors.
///
/// The ErrorType enumeration defines symbolic constants for different types of
/// runtime errors that can occur in the program. Each constant corresponds to a
/// specific type of error.
enum ErrorType {
  NamedObjectIsNotACamera,          ///< Named object is not a camera.
  SceneHasNoMainCameraSelected,     ///< Scene has no main camera selected.
  NullPointerToObject,              ///< Null pointer to object.
  ChildIsSameAsParent,              ///< Child is the same as parent.
  ObjectWithSameNameAlreadyInScene, ///< Object with same name already in scene.
  ChildHasSameNameAsParent,         ///< Child has the same name as parent.
};

/// @brief Array containing error messages corresponding to ErrorType values.
///
/// The errorMessages array holds C-style strings representing the error
/// messages for each ErrorType. Each error message corresponds to a specific
/// error type represented by the ErrorType enumeration.
static const char *errorMessages[]{
    "Named object is not a camera.",
    "Scene has no main camera selected.",
    "Null pointer to object.",
    "Child is same as parent.",
    "Object with same name already in scene.",
    "Child has same name as parent.",
};

/// @tparam errorType The specific ErrorType value that this RuntimeError
/// represents.
/// @class RuntimeError
/// @brief A templated runtime error class to handle specific error types.
///
/// The RuntimeError class is a template class used to handle specific runtime
/// errors. It inherits from the std::exception class and provides custom error
/// messages based on the errorType specified as the template parameter. The
/// what() method is overridden to return the corresponding error message.
template <ErrorType errorType> class RuntimeError : public std::exception {
public:
  /// @brief Returns the custom error message corresponding to the errorType.
  ///
  /// The what() method is overridden to provide a custom error message based on
  /// the errorType specified as the template parameter. It returns a C-style
  /// string containing the error message.
  ///
  /// @return A C-style string representing the custom error message for this
  /// RuntimeError.
  [[nodiscard]] const char *what() const noexcept override {
    return errorMessages[errorType];
  }
};

#endif // VERY_BASIC_ASCII_GRAPHICS_API_INCLUDE_UTIL_ERROR_HANDLING_HPP
