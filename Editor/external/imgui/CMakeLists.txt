cmake_minimum_required(VERSION 3.16)
project(imgui LANGUAGES CXX)

# Lista de todos los cpp que necesita ImGui
set(IMGUI_SOURCES
    imgui.cpp
    imgui_draw.cpp
    imgui_tables.cpp
    imgui_widgets.cpp
    imgui_demo.cpp
    backends/imgui_impl_glfw.cpp
    backends/imgui_impl_opengl3.cpp
    misc/cpp/imgui_stdlib.cpp
)

# Crear librería estática
add_library(imgui STATIC ${IMGUI_SOURCES})

# Includes de ImGui
target_include_directories(imgui PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/backends
)

# Dependencias externas
find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)

if (UNIX)
    find_package(X11 REQUIRED)
    target_link_libraries(imgui PUBLIC X11::X11)
endif()
# Linkear dependencias necesarias
target_link_libraries(imgui PUBLIC OpenGL::GL glfw)

# Opcional: define el estándar C++
target_compile_features(imgui PUBLIC cxx_std_17)
