#include "input/input.hpp"
#include "tests/animation_test.hpp"

int WinMain(HINSTANCE instance, HINSTANCE prevInstance, LPSTR cmd, int mode) {
  testAnimations(instance);
  return 0;
}
