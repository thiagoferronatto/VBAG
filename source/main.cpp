#include "tests/animation_test.hpp"

/// @brief Entry point of the program.
///
/// @return The `main` function does not return a value as it is marked as
/// `[[noreturn]]`.
[[noreturn]] int main() {
  testAnimations();
  // animations can't stop playing yet
}
