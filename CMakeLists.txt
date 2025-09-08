cmake_minimum_required(VERSION 3.16)

project(HydroEngine)

# Configurar C++ estándar
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Configurar directorios de salida
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# Configurar directorios de inclusión
include_directories(${CMAKE_SOURCE_DIR}/external/include)
include_directories(${CMAKE_SOURCE_DIR}/engine)

# Buscar OpenGL (requerido)
find_package(OpenGL REQUIRED)

# Recopilar archivos fuente del engine incluyendo STB image implementation
file(GLOB_RECURSE ENGINE_SOURCES
    "engine/Graphics/*.cpp"
    "engine/Rendering/*.cpp"
    "engine/Window/*.cpp"
    "engine/Core/*.cpp"
    "engine/Scene/*.cpp"
    "external/glad.c"

    "external/stb_image_impl.c"
)

# Crear biblioteca del engine
add_library(HydroEngine_Core STATIC ${ENGINE_SOURCES})

# Configurar directorios de inclusión para la biblioteca
target_include_directories(HydroEngine_Core
    PUBLIC
    ${CMAKE_SOURCE_DIR}/external/include
    ${CMAKE_SOURCE_DIR}/engine
)

# Enlazar OpenGL
target_link_libraries(HydroEngine_Core PUBLIC ${OPENGL_LIBRARIES})

# Configuraciones específicas por compilador
if(MINGW)
    # Para MinGW: necesitamos una versión compatible de GLFW
    message(STATUS "Detectado MinGW - configurando para compatibilidad")

    # Buscar GLFW con pkg-config o usar vcpkg
    find_package(PkgConfig QUIET)
    if(PkgConfig_FOUND)
        pkg_check_modules(GLFW QUIET glfw3)
        if(GLFW_FOUND)
            target_link_libraries(HydroEngine_Core PUBLIC ${GLFW_LIBRARIES})
            target_include_directories(HydroEngine_Core PUBLIC ${GLFW_INCLUDE_DIRS})
            message(STATUS "Usando GLFW desde pkg-config")
        endif()
    endif()

    # Si no se encuentra con pkg-config, usar vcpkg o compilar desde fuente
    if(NOT GLFW_FOUND)
        # Buscar en vcpkg
        find_library(GLFW_LIBRARY_MINGW
            NAMES glfw3 libglfw3.a
            PATHS
            "C:/vcpkg/installed/x64-mingw-static/lib"
            "C:/vcpkg/installed/x64-mingw-dynamic/lib"
            "C:/msys64/mingw64/lib"
            "${CMAKE_SOURCE_DIR}/external/lib/mingw"
        )

        if(GLFW_LIBRARY_MINGW)
            target_link_libraries(HydroEngine_Core PUBLIC ${GLFW_LIBRARY_MINGW})
            message(STATUS "Usando GLFW para MinGW: ${GLFW_LIBRARY_MINGW}")
        else()
            message(WARNING "No se encontró GLFW compatible con MinGW. Instala con: vcpkg install glfw3:x64-mingw-static")
            message(WARNING "O compila GLFW desde fuente con MinGW")
        endif()
    endif()

    # Bibliotecas específicas de MinGW
    target_link_libraries(HydroEngine_Core PUBLIC
        opengl32
        gdi32
        user32
        kernel32
        shell32
    )

elseif(MSVC)
    # Para MSVC: usar la biblioteca existente
    set(GLFW_LIBRARY "${CMAKE_SOURCE_DIR}/external/lib/glfw3.lib")
    target_link_libraries(HydroEngine_Core PUBLIC ${GLFW_LIBRARY})

    # Bibliotecas específicas de Windows/MSVC
    target_link_libraries(HydroEngine_Core PUBLIC
        opengl32
        gdi32
        user32
        kernel32
        shell32
    )

    message(STATUS "Usando GLFW para MSVC: ${GLFW_LIBRARY}")
endif()

# Crear ejecutable principal
add_executable(HydroEngine_Main engine/main.cpp)

# Enlazar el ejecutable con la biblioteca del engine
target_link_libraries(HydroEngine_Main PRIVATE HydroEngine_Core)

# Configuraciones específicas de Windows
if(WIN32)
    # Definiciones de compilador para Windows
    target_compile_definitions(HydroEngine_Core PRIVATE
        WIN32_LEAN_AND_MEAN
        NOMINMAX
    )
endif()

# Copiar recursos al directorio de salida
add_custom_target(copy_resources ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/resources
    ${CMAKE_BINARY_DIR}/resources
    COMMENT "Copying resources"
)

# Hacer que el ejecutable dependa de la copia de recursos
add_dependencies(HydroEngine_Main copy_resources)

# Configuración de release/debug
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_definitions(HydroEngine_Core PRIVATE DEBUG)
else()
    target_compile_definitions(HydroEngine_Core PRIVATE NDEBUG)
endif()
