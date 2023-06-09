#ifndef VERY_BASIC_ASCII_GRAPHICS_API_INCLUDE_DATA_TRIPLET_HPP
#define VERY_BASIC_ASCII_GRAPHICS_API_INCLUDE_DATA_TRIPLET_HPP

template <typename First, typename Second, typename Third>
struct GeneralTriplet {
  First first;
  Second second;
  Third third;
};

template <typename T> using Triplet = GeneralTriplet<T, T, T>;

#endif // VERY_BASIC_ASCII_GRAPHICS_API_INCLUDE_DATA_TRIPLET_HPP
