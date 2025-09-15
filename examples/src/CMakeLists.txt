cmake_minimum_required(VERSION 3.16)
project(HydroEngine_Main)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Incluir la librería
add_subdirectory(${CMAKE_SOURCE_DIR}/../../HydroEngine ${CMAKE_BINARY_DIR}/HydroEngine_build)

# Ejecutable principal
add_executable(HydroEngine_Main tests/main.cpp)

target_link_libraries(HydroEngine_Main PRIVATE HydroEngine_Core)

# Copiar recursos
add_custom_target(copy_resources ALL
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/resources
        ${CMAKE_BINARY_DIR}/resources
        COMMENT "Copying resources"
)

add_dependencies(HydroEngine_Main copy_resources)
