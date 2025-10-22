cmake_minimum_required(VERSION 3.30)
project(Core CXX)

# Estándar moderno
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Archivos fuente
file(GLOB_RECURSE SOURCES src/*.cpp)

set(GLAD_DIR ${CMAKE_CURRENT_SOURCE_DIR}/external/glad)
include_directories(${GLAD_DIR}/include)

# --- Librería principal ---
add_library(Core SHARED ${SOURCES})

# Definiciones para Windows
if (WIN32)
    target_compile_definitions(Core PRIVATE NOMINMAX)
endif()

# Includes comunes
target_include_directories(Core PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include/Core
    ${CMAKE_CURRENT_SOURCE_DIR}/external/include
    ${GLAD_DIR}/include
)

# --- Dependencias ---
if(WIN32)
    find_package(OpenGL REQUIRED)
    find_package(glfw3 CONFIG REQUIRED)
    set(assimp_DIR "C:/Program Files/assimp/cmake")
    find_package(assimp REQUIRED)

    target_link_libraries(Core PUBLIC
        OpenGL::GL
        glfw
        assimp::assimp
    )

elseif(UNIX)
    find_package(OpenGL REQUIRED)
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(GLFW REQUIRED glfw3)

    target_include_directories(Core PRIVATE ${GLFW_INCLUDE_DIRS})

    target_link_libraries(Core PUBLIC
        OpenGL::GL
        ${GLFW_LIBRARIES}
        X11 pthread dl m
        assimp
    )
endif()
