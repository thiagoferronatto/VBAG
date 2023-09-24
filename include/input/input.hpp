#ifndef VERY_BASIC_ASCII_GRAPHICS_API_INCLUDE_INPUT_HPP
#define VERY_BASIC_ASCII_GRAPHICS_API_INCLUDE_INPUT_HPP

#include <windows.h>

namespace vbag {

/// @enum KeyCode
/// @brief Enumerates the different key codes for keyboard keys.
///
/// The KeyCode enum represents the key codes for keyboard keys. Each enum
/// member is associated with a specific key, either an alphabetical character,
/// a numeric digit, or a special key such as Space, Escape, Shift, or Control.
/// The enum values correspond to the ASCII codes of the characters or virtual
/// key codes of the special keys.
enum class KeyCode {
  A = 'A',             ///< The key code for the 'A' key.
  B = 'B',             ///< The key code for the 'B' key.
  C = 'C',             ///< The key code for the 'C' key.
  D = 'D',             ///< The key code for the 'D' key.
  E = 'E',             ///< The key code for the 'E' key.
  F = 'F',             ///< The key code for the 'F' key.
  G = 'G',             ///< The key code for the 'G' key.
  H = 'H',             ///< The key code for the 'H' key.
  I = 'I',             ///< The key code for the 'I' key.
  J = 'J',             ///< The key code for the 'J' key.
  K = 'K',             ///< The key code for the 'K' key.
  L = 'L',             ///< The key code for the 'L' key.
  M = 'M',             ///< The key code for the 'M' key.
  N = 'N',             ///< The key code for the 'N' key.
  O = 'O',             ///< The key code for the 'O' key.
  P = 'P',             ///< The key code for the 'P' key.
  Q = 'Q',             ///< The key code for the 'Q' key.
  R = 'R',             ///< The key code for the 'R' key.
  S = 'S',             ///< The key code for the 'S' key.
  T = 'T',             ///< The key code for the 'T' key.
  U = 'U',             ///< The key code for the 'U' key.
  V = 'V',             ///< The key code for the 'V' key.
  W = 'W',             ///< The key code for the 'W' key.
  X = 'X',             ///< The key code for the 'X' key.
  Y = 'Y',             ///< The key code for the 'Y' key.
  Z = 'Z',             ///< The key code for the 'Z' key.
  One = '1',           ///< The key code for the '1' key.
  Two = '2',           ///< The key code for the '2' key.
  Three = '3',         ///< The key code for the '3' key.
  Four = '4',          ///< The key code for the '4' key.
  Five = '5',          ///< The key code for the '5' key.
  Six = '6',           ///< The key code for the '6' key.
  Seven = '7',         ///< The key code for the '7' key.
  Eight = '8',         ///< The key code for the '8' key.
  Nine = '9',          ///< The key code for the '9' key.
  Zero = '0',          ///< The key code for the '0' key.
  Space = VK_SPACE,    ///< The virtual key code for the Space key.
  Escape = VK_ESCAPE,  ///< The virtual key code for the Escape key.
  LShift = VK_LSHIFT,  ///< The virtual key code for the Left Shift key.
  RShift = VK_RSHIFT,  ///< The virtual key code for the Right Shift key.
  LCtrl = VK_LCONTROL, ///< The virtual key code for the Left Control key.
  RCtrl = VK_RCONTROL  ///< The virtual key code for the Right Control key.
};

/// @class Input
/// @brief The Input class provides static functions to handle keyboard input.
///
/// The Input class allows checking the state of keyboard keys. It provides a
/// static function `getKey` to determine if a specific key is currently pressed
/// or not.
class Input {
public:
  /// @brief Checks if the specified key is currently pressed.
  ///
  /// This function checks whether a given key is currently pressed or not. It
  /// returns true if the key is pressed, and false otherwise.
  ///
  /// @param keyCode The KeyCode representing the key to check.
  /// @return True if the specified key is currently pressed, false otherwise.
  static bool getKey(KeyCode);
};

} // namespace vbag

#endif // VERY_BASIC_ASCII_GRAPHICS_API_INCLUDE_INPUT_HPP
