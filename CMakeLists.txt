cmake_minimum_required(VERSION 3.20)
project(HydroEngine VERSION 1.0.0 LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

if(WIN32)
    set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
endif()

include_directories(
    ${CMAKE_SOURCE_DIR}/external/include
    ${CMAKE_SOURCE_DIR}/engine
)

link_directories(
    ${CMAKE_SOURCE_DIR}/external/lib
)

file(GLOB_RECURSE ENGINE_SOURCES
    "engine/Core/*.cpp"
    "engine/Core/*.h"
    "engine/Graphics/*.cpp"
    "engine/Graphics/*.h"
    "engine/Scene/*.cpp"
    "engine/Scene/*.h"
    "engine/Window/*.cpp"
    "engine/Window/*.h"
)

set(EXTERNAL_SOURCES
    external/glad.c
        engine/Graphics/GraphicsContext.cpp
)

add_library(HydroEngine_Core STATIC
    ${ENGINE_SOURCES}
    ${EXTERNAL_SOURCES}
)

target_include_directories(HydroEngine_Core PUBLIC
    ${CMAKE_SOURCE_DIR}/external/include
    ${CMAKE_SOURCE_DIR}/engine
)

target_link_libraries(HydroEngine_Core
    glfw3
    opengl32
    gdi32
    user32
    kernel32
)

target_compile_definitions(HydroEngine_Core PRIVATE
    $<$<CONFIG:Debug>:DEBUG>
    $<$<CONFIG:Debug>:_DEBUG>
)

if(MSVC)
    target_compile_options(HydroEngine_Core PRIVATE
        /W4
        $<$<CONFIG:Debug>:/Od>
        $<$<CONFIG:Release>:/O2>
    )
else()
    target_compile_options(HydroEngine_Core PRIVATE
        -Wall -Wextra -Wpedantic
        $<$<CONFIG:Debug>:-g>
        $<$<CONFIG:Release>:-O3>
    )
endif()








# Configuración para copiar recursos
if(EXISTS "${CMAKE_SOURCE_DIR}/resources")
    file(COPY ${CMAKE_SOURCE_DIR}/resources DESTINATION ${CMAKE_BINARY_DIR})
endif()


