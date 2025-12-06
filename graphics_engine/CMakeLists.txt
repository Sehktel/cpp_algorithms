cmake_minimum_required(VERSION 3.10)
project(GraphicsEngine)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Исходные файлы
set(CORE_SOURCES
    core/math/vector3.cpp
    core/math/matrix4.cpp
    core/renderer/framebuffer.cpp
    core/camera/camera.cpp
)

set(RASTERIZATION_SOURCES
    rasterization/bresenham/line.cpp
    rasterization/bresenham/circle.cpp
    rasterization/polygons/fill.cpp
)

set(TEXTURE_SOURCES
    textures/texture.cpp
)

set(LIGHTING_SOURCES
    lighting/phong.cpp
    lighting/gouraud.cpp
)

set(GEOMETRY_SOURCES
    geometry/transforms/affine.cpp
    geometry/normals/normals.cpp
    geometry/generators/mesh_generator.cpp
    geometry/clipping/clipping.cpp
)

# Исполняемые файлы
add_executable(basic_example
    examples/basic_example.cpp
    ${CORE_SOURCES}
    ${RASTERIZATION_SOURCES}
)

add_executable(torus_example
    examples/torus_example.cpp
    ${CORE_SOURCES}
    ${RASTERIZATION_SOURCES}
    ${GEOMETRY_SOURCES}
)

# Включаемые директории
target_include_directories(basic_example PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_include_directories(torus_example PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
)

