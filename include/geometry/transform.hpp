#ifndef VERY_BASIC_ASCII_GRAPHICS_API_INCLUDE_GEOMETRY_TRANSFORM_HPP
#define VERY_BASIC_ASCII_GRAPHICS_API_INCLUDE_GEOMETRY_TRANSFORM_HPP

#include "math/matrix.hpp"
#include "math/vector.hpp"
#include "util/floating_point_comparison.hpp"

class Object;

/// @class Transform
/// @brief The Transform class represents an object's transformation in 3D
/// space.
///
/// The Transform class stores the transformation matrix of an object, which
/// includes scaling, rotation, and translation. It provides functions to apply
/// transformations to an object's position and orientation and retrieve the
/// object's forward, right, and up vectors, as well as the translation
/// components.
class Transform {
public:
  /// @brief Constructs a Transform object associated with the given Object.
  ///
  /// @param object A pointer to the Object to which this Transform belongs.
  explicit Transform(Object *);

  /// @brief Constructs a Transform object with the given transformation matrix
  /// and associated Object.
  ///
  /// @param matrix The 4x4 transformation matrix representing scaling,
  /// rotation, and translation.
  /// @param object A pointer to the Object to which this Transform belongs.
  Transform(const M4F &, Object *);

  /// @brief Returns the value at the specified index in the transformation
  /// matrix.
  ///
  /// @param index The index (0 to 15) of the value to retrieve.
  /// @return The value at the specified index in the transformation matrix.
  [[nodiscard]] float operator[](size_t) const;

  /// @brief Returns the value at the specified row and column in the
  /// transformation matrix.
  ///
  /// @param row The row index (0 to 3) of the value to retrieve.
  /// @param col The column index (0 to 3) of the value to retrieve.
  /// @return The value at the specified row and column in the transformation
  /// matrix.
  [[nodiscard]] float operator()(size_t, size_t) const;

  /// @brief Applies the transformation to a 3D vector and returns the resulting
  /// vector.
  ///
  /// @param vector The 3D vector to transform.
  /// @return The transformed 3D vector.
  [[nodiscard]] V3F operator*(const V3F &) const;

  /// @brief Applies the transformation to another Transform and returns the
  /// resulting matrix.
  ///
  /// @param other The Transform to be multiplied with this Transform.
  /// @return The resulting transformation matrix after the multiplication.
  [[nodiscard]] M4F operator*(const Transform &) const;

  /// @brief Scales the object by the given scaling factors in each axis.
  ///
  /// @param scales A 3D vector containing the scaling factors for the x, y, and
  /// z axes.
  void scale(V3F);

  /// @brief Scales the object by the given scaling factors in each axis.
  ///
  /// @param xs The scaling factor for the x-axis.
  /// @param ys The scaling factor for the y-axis.
  /// @param zs The scaling factor for the z-axis.
  void scale(float, float, float);

  /// @brief Rotates the object by the given Euler angles (yaw, pitch, roll).
  ///
  /// @param eulerAngles A 3D vector containing the Euler angles (yaw, pitch,
  /// roll) for the rotation.
  void rotate(V3F);

  /// @brief Rotates the object by the given Euler angles (yaw, pitch, roll).
  ///
  /// @param xr The rotation angle around the y-axis (yaw) in radians.
  /// @param yr The rotation angle around the x-axis (pitch) in radians.
  /// @param zr The rotation angle around the z-axis (roll) in radians.
  void rotate(float, float, float);

  /// @brief Rotates the object in-place by the given Euler angles (yaw, pitch,
  /// roll).
  ///
  /// @param eulerAngles A 3D vector containing the Euler angles (yaw, pitch,
  /// roll) for the rotation.
  void rotateInPlace(V3F);

  /// @brief Rotates the object in-place by the given Euler angles (yaw, pitch,
  /// roll).
  ///
  /// @param xr The rotation angle around the y-axis (yaw) in radians.
  /// @param yr The rotation angle around the x-axis (pitch) in radians.
  /// @param zr The rotation angle around the z-axis (roll) in radians.
  void rotateInPlace(float, float, float);

  /// @brief Translates the object by the given translation vector.
  ///
  /// @param translation A 3D vector representing the translation in x, y, and z
  /// axes.
  void translate(V3F);

  /// @brief Translates the object by the given translation values in x, y, and
  /// z axes.
  ///
  /// @param xt The translation in the x-axis.
  /// @param yt The translation in the y-axis.
  /// @param zt The translation in the z-axis.
  void translate(float, float, float);

  /// @brief Returns the forward vector of the object's orientation.
  ///
  /// @return The forward vector of the object's orientation.
  [[nodiscard]] V3F forward() const;

  /// @brief Returns the right vector of the object's orientation.
  ///
  /// @return The right vector of the object's orientation.
  [[nodiscard]] V3F right() const;

  /// @brief Returns the up vector of the object's orientation.
  ///
  /// @return The up vector of the object's orientation.
  [[nodiscard]] V3F up() const;

  /// @brief Returns the translation components of the transformation as a 3D
  /// vector.
  ///
  /// @return The translation components of the transformation.
  [[nodiscard]] V3F translation() const;

  /// @brief Returns the x-coordinate of the translation.
  ///
  /// @return The x-coordinate of the translation.
  [[nodiscard]] float x() const { return transform_[3]; }

  /// @brief Returns the y-coordinate of the translation.
  ///
  /// @return The y-coordinate of the translation.
  [[nodiscard]] float y() const { return transform_[7]; }

  /// @brief Returns the z-coordinate of the translation.
  ///
  /// @return The z-coordinate of the translation.
  [[nodiscard]] float z() const { return transform_[11]; }

  /// @brief Friend function to perform matrix multiplication between a matrix
  /// and a Transform.
  ///
  /// @param matrix The 4x4 matrix to be multiplied with the Transform.
  /// @param transform The Transform to be multiplied with the matrix.
  /// @return The resulting 4x4 matrix after the multiplication.
  [[nodiscard]] friend M4F operator*(const M4F &matrix,
                                     const Transform &transform) {
    return matrix * transform.transform_;
  }

private:
  M4F transform_{1, 0, 0, 0,  //
                 0, 1, 0, 0,  //
                 0, 0, 1, 0,  //
                 0, 0, 0, 1}; ///< The 4x4 transformation matrix.
  Object *object_;            ///< Pointer to the associated Object.
};

#endif // VERY_BASIC_ASCII_GRAPHICS_API_INCLUDE_GEOMETRY_TRANSFORM_HPP
