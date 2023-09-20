#ifndef VERY_BASIC_ASCII_GRAPHICS_API_INCLUDE_UTIL_STRING_HPP
#define VERY_BASIC_ASCII_GRAPHICS_API_INCLUDE_UTIL_STRING_HPP

#include <string>

template <typename T>
concept Character = std::same_as<T, char> || std::same_as<T, unsigned char> ||
                    std::same_as<T, signed char> || std::same_as<T, char8_t> ||
                    std::same_as<T, char16_t> || std::same_as<T, char32_t> ||
                    std::same_as<T, wchar_t>;

template <Character T> class BasicString {
public:
  constexpr BasicString() = default;

  constexpr BasicString(const BasicString &other) : size_{other.size_} {
    if (!other.isSmall())
      data_ = new T[other.size_];
    auto ptr{data_};
    for (auto c : other)
      *ptr++ = c;
  }

  constexpr BasicString(BasicString &&other) noexcept : size_{other.size_} {
    if (other.isSmall()) {
      auto ptr{data_};
      for (auto c : other)
        *ptr++ = c;
    } else {
      data_ = other.data_;
      other.data_ = nullptr;
    }
    other.size_ = 0;
  }

  constexpr BasicString(const T *cstr) {
    constexpr auto maxStdStringSize{std::string{}.max_size()};
    constexpr auto zero{T{}};
    for (size_t i{}; i < maxStdStringSize; ++i) {
      if (cstr[i] == zero)
        break;
      ++size_;
    }
    if (size_ > ssoTreshold_)
      data_ = new T[size_];
    for (size_t i{}; i < size_; ++i)
      data_[i] = *cstr++;
  }

  constexpr BasicString(const std::basic_string<T> &string) {
    if ((size_ = string.size()) > ssoTreshold_)
      data_ = new T[size_];
    auto ptr{data_};
    for (auto c : string)
      *ptr++ = c;
  }

  constexpr ~BasicString() {
    if (!isSmall())
      delete[] data_;
  }

  constexpr BasicString &operator=(const BasicString &other) {
    if (this == &other)
      return *this;
    if (isSmall()) {
      if (!other.isSmall())
        data_ = new T[other.size_];
    } else {
      if (other.isSmall()) {
        delete[] data_;
        data_ = buffer_;
      } else if (size_ != other.size_) {
        delete[] data_;
        data_ = new T[other.size_];
      }
    }
    auto ptr{data_};
    for (auto c : other)
      *ptr++ = c;
    size_ = other.size_;
    return *this;
  }

  constexpr BasicString &operator=(BasicString &&other) noexcept {
    if (this == &other)
      return *this;
    if (isSmall()) {
      if (other.isSmall()) {
        auto ptr{data_};
        for (auto c : other)
          *ptr++ = c;
      } else {
        data_ = other.data_;
        other.data_ = other.buffer_;
      }
    } else {
      delete[] data_;
      if (other.isSmall()) {
        data_ = buffer_;
        auto ptr{data_};
        for (auto c : other)
          *ptr++ = c;
      } else {
        data_ = other.data_;
        other.data_ = other.buffer_;
      }
    }
    size_ = other.size_;
    other.size_ = 0;
    return *this;
  }

  constexpr BasicString operator+(const BasicString &other) const {
    BasicString result{size_ + other.size_};
    auto ptr{result.data_};
    for (auto c : *this)
      *ptr++ = c;
    for (auto c : other)
      *ptr++ = c;
    return result;
  }

  constexpr BasicString &operator+=(const BasicString &other) {
    return *this = *this + other;
  }

  constexpr const T *begin() const { return data_; }

  constexpr const T *end() const { return data_ + size_; }

private:
  constexpr BasicString(size_t size) : size_{size} {
    if (size_ > ssoTreshold_)
      data_ = new T[size_];
  }

  [[nodiscard]] constexpr bool isSmall() const { return data_ == buffer_; }

  static constexpr size_t ssoTreshold_{16};

  T *data_{buffer_};
  T buffer_[ssoTreshold_]{};
  size_t size_{};
};

using String = BasicString<char>;

#endif // VERY_BASIC_ASCII_GRAPHICS_API_INCLUDE_UTIL_STRING_HPP
