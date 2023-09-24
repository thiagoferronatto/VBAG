#include "input/input.hpp"

namespace vbag {

bool Input::getKey(KeyCode keyCode) {
  return GetAsyncKeyState(int(keyCode)) & 0x8000;
}

} // namespace vbag