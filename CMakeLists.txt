cmake_minimum_required(VERSION 3.30)
project(Pruebas CXX)

file(GLOB SOURCES src/*.cpp)

add_executable(Pruebas ${SOURCES})

# Includes
target_include_directories(Pruebas PRIVATE
    external/include
    src/include
)

# OpenGL
find_package(OpenGL REQUIRED)
target_link_libraries(Pruebas PRIVATE OpenGL::GL)

find_package(PkgConfig REQUIRED)
pkg_check_modules(GLFW REQUIRED glfw3)

target_include_directories(Pruebas PRIVATE ${GLFW_INCLUDE_DIRS})
target_link_libraries(Pruebas PRIVATE ${GLFW_LIBRARIES})

# Dependencias linux
target_link_libraries(Pruebas PRIVATE X11 pthread dl m)
