cmake_minimum_required(VERSION 3.30)
project(Core CXX)

# Estándar moderno
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Archivos fuente
file(GLOB_RECURSE SOURCES src/*.cpp)

# Ejecutable
add_executable(Core ${SOURCES})

# Definiciones para Windows
if (WIN32)
    target_compile_definitions(Core PRIVATE NOMINMAX)
endif()

# Includes comunes
target_include_directories(Core PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Core
        ${CMAKE_CURRENT_SOURCE_DIR}/external/include
)

# --- Dependencias ---
if(WIN32)
    # OpenGL y GLFW
    find_package(OpenGL REQUIRED)
    find_package(glfw3 CONFIG REQUIRED)

    # Ruta de Assimp compilado
    set(assimp_DIR "C:/Program Files/assimp/cmake")  # <- Cambiar según tu instalación
    find_package(assimp REQUIRED)

    # Link de librerías
    target_link_libraries(Core PRIVATE
            OpenGL::GL
            glfw
            assimp::assimp
    )

elseif(UNIX)
    # OpenGL
    find_package(OpenGL REQUIRED)
    target_link_libraries(Core PRIVATE OpenGL::GL)

    # GLFW vía pkg-config
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(GLFW REQUIRED glfw3)
    target_include_directories(Core PRIVATE ${GLFW_INCLUDE_DIRS})
    target_link_libraries(Core PRIVATE ${GLFW_LIBRARIES})

    # Dependencias del sistema
    target_link_libraries(Core PRIVATE X11 pthread dl m)

    # Assimp en Linux
    find_package(assimp REQUIRED)
    target_link_libraries(Core PRIVATE assimp)
endif()
