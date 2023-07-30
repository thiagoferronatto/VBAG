#ifndef VERY_BASIC_ASCII_GRAPHICS_API_INCLUDE_SCREEN_HPP
#define VERY_BASIC_ASCII_GRAPHICS_API_INCLUDE_SCREEN_HPP

#include <cstdint>
#include <cstdio>
#include <cstring>

/// @brief represents an character matrix that can be used as a screen
/// @tparam T the type of character used
template <typename T> class BasicScreen {
public:
  [[maybe_unused]] BasicScreen(size_t width, size_t height,
                               FILE *stream = stdout)
      : buffer_{new T[(width + 1) * height]}, width_{width + 1U},
        height_{height}, stream_{stream} {}

  ~BasicScreen() { delete[] buffer_; }

  auto fill(T c) {
    memset(buffer_, c, width_ * height_);
    const auto end = buffer_ + height_ * width_;
    for (auto ptr{buffer_ + width_ - 1}; ptr < end; ptr += width_)
      *ptr = '\n';
  }

  auto inline clear() { fputs("\033c", stream_); }

  [[maybe_unused]] inline auto get(size_t row, size_t col) const {
    return buffer_[row * width_ + col];
  }

  inline auto set(size_t row, size_t col, T c) {
    buffer_[row * width_ + col] = c;
  }

  auto show() const { fwrite(buffer_, sizeof(T), height_ * width_, stream_); }

  inline auto width() const { return width_ - 1U; }
  inline auto height() const { return height_; }

  // this is supposed to account for the fact that characters are way closer
  // horizontally than vertically in most console windows; this is not a one
  // size fits all solution, but no one else is gonna try this anyways
  static constexpr auto stretchFactor{0.55f};

private:
  T *const buffer_;
  size_t width_, height_;
  FILE *const stream_{stdout};
};

using Screen = BasicScreen<char>;

#endif // VERY_BASIC_ASCII_GRAPHICS_API_INCLUDE_SCREEN_HPP
