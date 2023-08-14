cmake_minimum_required(VERSION 3.18)

project(very_basic_ascii_graphics_api
        VERSION 0.1
        LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)

# stuff to compile the app icon along with the executable (remove in case of any problems)
add_custom_command(
        OUTPUT icon/vbag_icon.o
        COMMAND windres ../media/images/icon/vbag_icon.rc -o icon/vbag_icon.o
        DEPENDS ../media/images/icon/vbag_icon.rc
)

include_directories(include)

add_executable(VBAG WIN32
        icon/vbag_icon.o # links the icon to the binary (remove if any trouble arises)
        include/animation/animation_engine.hpp
        include/geometry/graph.hpp
        include/math/vector.hpp
        include/output/d3d9_screen.hpp
        include/tests/animation_test.hpp
        source/animation/animation_engine.cpp
        source/tests/animation_test.cpp
        source/main.cpp
        include/util/floating_point_comparison.hpp
        include/math/matrix.hpp
        include/geometry/object.hpp
        source/graphics/camera.cpp
        include/graphics/camera.hpp
        source/geometry/transform.cpp
        include/geometry/transform.hpp
        source/geometry/scene.cpp
        include/geometry/scene.hpp
        include/util/error_handling.hpp
        source/input/input.cpp
        include/input/input.hpp
        source/geometry/object.cpp source/graphics/triangle_mesh.cpp include/graphics/triangle_mesh.hpp source/graphics/light.cpp include/graphics/light.hpp)

target_link_libraries(VBAG d3d9.lib)