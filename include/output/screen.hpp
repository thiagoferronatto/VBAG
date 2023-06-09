#ifndef VERY_BASIC_ASCII_GRAPHICS_API_INCLUDE_SCREEN_HPP
#define VERY_BASIC_ASCII_GRAPHICS_API_INCLUDE_SCREEN_HPP

#include <cstdint>
#include <cstdio>

/// @brief represents an character matrix that can be used as a screen
/// @tparam T the type of character used
template <typename T> class BasicScreen {
public:
  [[maybe_unused]] BasicScreen(size_t width, size_t height,
                               FILE *stream = stdout)
      : buffer_{new T[width * height]}, width_{width}, height_{height},
        stream_{stream} {}

  ~BasicScreen() { delete[] buffer_; }

  auto fill(T c) {
    for (size_t i{}; i < width_ * height_; ++i)
      buffer_[i] = c;
  }

  auto inline clear() { fputs("\033c", stream_); }

  [[maybe_unused]] inline auto get(size_t row, size_t col) const {
    return buffer_[row * width_ + col];
  }

  inline auto set(size_t row, size_t col, T c) {
    buffer_[row * width_ + col] = c;
  }

  auto show() const {
    auto end = buffer_ + height_ * width_;
    for (auto ptr{buffer_}; ptr < end; ptr += width_) {
      fwrite(ptr, sizeof(T), width_, stream_);
      fputc('\n', stream_);
    }
  }

  inline auto width() const { return width_; }
  inline auto height() const { return height_; }

private:
  T *const buffer_;
  size_t width_, height_;
  FILE *const stream_{stdout};
};

using Screen = BasicScreen<char>;

#endif // VERY_BASIC_ASCII_GRAPHICS_API_INCLUDE_SCREEN_HPP
