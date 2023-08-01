#ifndef VERY_BASIC_ASCII_GRAPHICS_API_INCLUDE_SCREEN_HPP
#define VERY_BASIC_ASCII_GRAPHICS_API_INCLUDE_SCREEN_HPP

#include <cstdint>
#include <cstdio>
#include <cstring>

/// @tparam T The type of the character used in the screen.
/// @class BasicScreen
/// @brief Represents a character matrix that can be used as a screen.
///
/// The BasicScreen class template provides a simple character matrix that can
/// be used as a screen representation. It can be filled with characters,
/// cleared, and displayed to the specified output stream.
template <typename T> class BasicScreen {
public:
  /// @brief Constructor to create a BasicScreen instance.
  ///
  /// @param width The width of the screen (number of characters per row).
  /// @param height The height of the screen (number of rows).
  /// @param stream The output stream to which the screen will be displayed
  /// (default is stdout).
  [[maybe_unused]] BasicScreen(size_t width, size_t height,
                               FILE *stream = stdout)
      : buffer_{new T[(width + 1) * height]}, width_{width + 1U},
        height_{height}, stream_{stream} {}

  /// @brief Destructor to release memory used by the screen buffer.
  ~BasicScreen() { delete[] buffer_; }

  /// @brief Fills the entire screen with the specified character.
  ///
  /// @param c The character used to fill the screen.
  auto fill(T c) {
    memset(buffer_, c, width_ * height_);
    const auto end = buffer_ + height_ * width_;
    for (auto ptr{buffer_ + width_ - 1}; ptr < end; ptr += width_)
      *ptr = '\n';
  }

  /// @brief Clears the screen by printing an escape sequence to the output
  /// stream.
  auto inline clear() { fputs(/*"\033c"*/ "\033[H", stream_); }

  /// @brief Gets the character at the specified position in the screen.
  ///
  /// @param row The row index of the character.
  /// @param col The column index of the character.
  /// @return The character at the specified position.
  [[maybe_unused]] inline auto get(size_t row, size_t col) const {
    return buffer_[row * width_ + col];
  }

  /// @brief Sets the character at the specified position in the screen.
  ///
  /// @param row The row index of the character.
  /// @param col The column index of the character.
  /// @param c The character to set at the specified position.
  inline auto set(size_t row, size_t col, T c) {
    buffer_[row * width_ + col] = c;
  }

  /// @brief Shows the screen by writing the buffer to the output stream.
  auto show() const { fwrite(buffer_, sizeof(T), height_ * width_, stream_); }

  /// @brief Gets the width of the screen.
  ///
  /// @return The width of the screen in characters.
  inline auto width() const { return width_ - 1U; }

  /// @brief Gets the height of the screen.
  ///
  /// @return The height of the screen in rows.
  inline auto height() const { return height_; }

  /// @brief The stretch factor to account for horizontal character spacing.
  ///
  /// This factor is used to adjust the horizontal spacing between characters,
  /// considering that characters are often closer horizontally than vertically
  /// in most console windows.
  ///
  /// @note Expected usage: incorporating this into a camera's aspect ratio.
  static constexpr auto stretchFactor{0.55f};

private:
  T *const buffer_; ///< The buffer representing the screen.
  size_t width_;    ///< The width of the screen (number of characters per row).
  size_t height_;   ///< The height of the screen (number of rows).
  FILE *const stream_{stdout}; ///< The output stream to which the screen is
                               ///< displayed (default is stdout).
};

/// @brief Type alias for a BasicScreen instance using characters of type
/// 'char'.
using Screen = BasicScreen<char>;

#endif // VERY_BASIC_ASCII_GRAPHICS_API_INCLUDE_SCREEN_HPP
