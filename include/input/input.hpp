#ifndef VERY_BASIC_ASCII_GRAPHICS_API_INCLUDE_INPUT_HPP
#define VERY_BASIC_ASCII_GRAPHICS_API_INCLUDE_INPUT_HPP

#include <windows.h>

enum class KeyCode {
  A = 'A',
  B = 'B',
  C = 'C',
  D = 'D',
  E = 'E',
  F = 'F',
  G = 'G',
  H = 'H',
  I = 'I',
  J = 'J',
  K = 'K',
  L = 'L',
  M = 'M',
  N = 'N',
  O = 'O',
  P = 'P',
  Q = 'Q',
  R = 'R',
  S = 'S',
  T = 'T',
  U = 'U',
  V = 'V',
  W = 'W',
  X = 'X',
  Y = 'Y',
  Z = 'Z',
  One = '1',
  Two = '2',
  Three = '3',
  Four = '4',
  Five = '5',
  Six = '6',
  Seven = '7',
  Eight = '8',
  Nine = '9',
  Zero = '0',
  Space = VK_SPACE,
  Escape = VK_ESCAPE,
  LShift = VK_LSHIFT,
  RShift = VK_RSHIFT,
  LCtrl = VK_LCONTROL,
  RCtrl = VK_RCONTROL
};

class Input {
public:
  static bool getKey(KeyCode);
};

#endif // VERY_BASIC_ASCII_GRAPHICS_API_INCLUDE_INPUT_HPP
