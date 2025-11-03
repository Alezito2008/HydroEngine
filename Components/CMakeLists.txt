cmake_minimum_required(VERSION 3.30)

project(Components CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_library(Components STATIC
    # TestComponent/TestComponent.cpp
    Transform/Transform.cpp
    Collectible/CollectibleComponent.cpp
)

target_include_directories(Components PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../ECS/include
)

find_package(glm REQUIRED)

target_link_libraries(Components PRIVATE
    ECS
    glm::glm
)
