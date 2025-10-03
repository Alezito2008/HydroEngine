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

# GLFW (via pkg-config)
find_package(PkgConfig REQUIRED)
pkg_check_modules(GLFW REQUIRED glfw3)

target_include_directories(Pruebas PRIVATE ${GLFW_INCLUDE_DIRS})
target_link_libraries(Pruebas PRIVATE ${GLFW_LIBRARIES})

# Assimp
find_package(assimp REQUIRED)
target_link_libraries(Pruebas PRIVATE assimp::assimp)

# Dependencias solo en Linux
if(UNIX AND NOT WIN32)
    target_link_libraries(Pruebas PRIVATE X11 pthread dl m)
endif()
