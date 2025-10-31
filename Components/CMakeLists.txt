cmake_minimum_required(VERSION 3.30)

project(Components CXX)

add_library(Components SHARED
    # TestComponent/TestComponent.cpp
    Transform/Transform.cpp
)

target_include_directories(Components PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../ECS/include
)

target_link_libraries(Components PRIVATE ECS)
