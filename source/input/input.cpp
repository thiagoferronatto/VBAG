#include "input/input.hpp"

bool Input::getKey(KeyCode keyCode) {
  return GetAsyncKeyState(int(keyCode)) & 0x8000;
}
