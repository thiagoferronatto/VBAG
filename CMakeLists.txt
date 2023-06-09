cmake_minimum_required(VERSION 3.18)

project(very_basic_ascii_graphics_api
  VERSION 0.1
  LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)

include_directories(include)

add_executable(VBAG
        source/main.cpp
        include/math/vector.hpp
        include/math/graph.hpp
        include/output/screen.hpp
        include/animation/animation_engine.hpp
        source/tests/animation_test.cpp
        include/tests/animation_test.hpp)
