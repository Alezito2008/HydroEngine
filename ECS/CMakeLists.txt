cmake_minimum_required(VERSION 3.30)
set(CMAKE_CXX_STANDARD 17)

project(ECS CXX)

add_library(ECS SHARED
    src/GameObject.cpp
    src/Component.cpp
    src/ComponentRegistry.cpp
    src/Scene.cpp
    src/SceneManager.cpp
)

target_include_directories(ECS PUBLIC
    ${PROJECT_SOURCE_DIR}/include/ECS
    ${PROJECT_SOURCE_DIR}/src
    ${PROJECT_SOURCE_DIR}/../Components
)

find_package(glm REQUIRED)
target_link_libraries(ECS PRIVATE glm::glm)

# EJecutable para probar cosas

# add_executable(Test src/main.cpp)
# target_link_libraries(Test PRIVATE ECS)


# target_include_directories(Test
#PUBLIC ${PROJECT_SOURCE_DIR}/include/ECS
#PRIVATE ${PROJECT_SOURCE_DIR}/private
#)
