cmake_minimum_required(VERSION 3.16)
project(HydroEngine_Core)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# Includes
include_directories(${CMAKE_SOURCE_DIR}/external/include)
include_directories(${CMAKE_SOURCE_DIR}/engine)

# Dependencias
find_package(OpenGL REQUIRED)

# Fuentes
file(GLOB_RECURSE ENGINE_SOURCES
        "engine/Graphics/*.cpp"
        "engine/Rendering/*.cpp"
        "engine/Window/*.cpp"
        "engine/Core/*.cpp"
        "engine/Scene/*.cpp"
        "external/glad.c"
        "external/stb_image_impl.c"
)

# Biblioteca
add_library(HydroEngine_Core STATIC ${ENGINE_SOURCES}
        engine/Graphics/IndexBuffer.cpp
        engine/Graphics/IndexBuffer.h
        engine/Core/Debug.cpp
        engine/Core/Debug.h
        engine/Graphics/VertexArray.cpp
        engine/Graphics/VertexArray.h
        engine/Graphics/VertexBuffer.cpp
        engine/Graphics/VertexBuffer.h
        engine/Graphics/VertexBufferLayout.h)

target_include_directories(HydroEngine_Core
        PUBLIC
        ${CMAKE_SOURCE_DIR}/external/include
        ${CMAKE_SOURCE_DIR}/engine
)

target_link_libraries(HydroEngine_Core
        PUBLIC
        OpenGL::GL
        ${CMAKE_SOURCE_DIR}/external/lib/glfw3.lib
)

# Definiciones
if(WIN32)
    target_compile_definitions(HydroEngine_Core PRIVATE
            WIN32_LEAN_AND_MEAN
            NOMINMAX
    )
endif()

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_definitions(HydroEngine_Core PRIVATE DEBUG)
else()
    target_compile_definitions(HydroEngine_Core PRIVATE NDEBUG)
endif()
