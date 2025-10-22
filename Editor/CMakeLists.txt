cmake_minimum_required(VERSION 3.30)

project(Editor)

add_executable(EditorMain
    src/main.cpp
    src/Interface.cpp
    src/DockLayout.cpp
    src/Themes.cpp
    src/Preferences.cpp
    src/Console.cpp

    src/Panels/Console/ConsolePanel.cpp
    src/Panels/Tree/TreePanel.cpp
    src/Panels/Inspector/InspectorPanel.cpp
    src/Panels/MenuBar/MenuBar.cpp
    
    src/Popups/ComponentSelector/ComponentSelectorPopup.cpp
)

target_include_directories(EditorMain PRIVATE
    external/include
    src/include
    src/Panels
    src/Popups
)

# Librerias y demás

target_include_directories(EditorMain PRIVATE
    ${PROJECT_SOURCE_DIR}/../ECS/include
    ${PROJECT_SOURCE_DIR}/../Core/include
)

target_link_libraries(EditorMain PRIVATE ECS Core)

add_subdirectory(${PROJECT_SOURCE_DIR}/external/imgui)

find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)

target_link_libraries(EditorMain PRIVATE
    OpenGL::GL
    glfw
    imgui
)
