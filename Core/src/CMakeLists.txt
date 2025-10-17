cmake_minimum_required(VERSION 3.30)
project(Pruebas CXX)

# Fuerza estándar moderno
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

file(GLOB SOURCES src/*.cpp)

add_executable(Pruebas ${SOURCES})

# Definiciones para evitar conflictos de macros de Windows
if (WIN32)
    target_compile_definitions(Pruebas PRIVATE NOMINMAX)
endif()

# Includes comunes
target_include_directories(Pruebas PRIVATE
        src/include
        external/include
)

if(WIN32)
    # Windows: usar vcpkg
    find_package(OpenGL REQUIRED)
    find_package(glfw3 CONFIG REQUIRED)
    find_package(assimp CONFIG REQUIRED)

    target_link_libraries(Pruebas PRIVATE
            OpenGL::GL
            glfw
            assimp::assimp
    )

elseif(UNIX)
    # Linux: usar pkg-config
    find_package(OpenGL REQUIRED)
    target_link_libraries(Pruebas PRIVATE OpenGL::GL)

    find_package(PkgConfig REQUIRED)
    pkg_check_modules(GLFW REQUIRED glfw3)

    target_include_directories(Pruebas PRIVATE ${GLFW_INCLUDE_DIRS})
    target_link_libraries(Pruebas PRIVATE ${GLFW_LIBRARIES})

    # Dependencias adicionales en Linux
    target_link_libraries(Pruebas PRIVATE X11 pthread dl m)

    # Assimp en Linux
    find_package(assimp REQUIRED)
    target_link_libraries(Pruebas PRIVATE assimp)
endif()