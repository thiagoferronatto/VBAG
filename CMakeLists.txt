cmake_minimum_required(VERSION 3.18)

project(very_basic_ascii_graphics_api
  VERSION 0.1
  LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)

include_directories(include)

add_executable(VBAG
        include/animation/animation_engine.hpp
        include/data/triplet.hpp
        include/geometry/graph.hpp
        include/math/vector.hpp
        include/output/screen.hpp
        include/tests/animation_test.hpp
        source/animation/animation_engine.cpp
        source/tests/animation_test.cpp
        source/main.cpp include/util/floating_point_comparison.hpp
        include/math/matrix.hpp
        include/geometry/object.hpp)
