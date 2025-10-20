#include <iostream>
#include <memory>

#include "ComponentManager.h"
#include "GameObject.h"
#include "GameObject.inl"
#include "SceneManager.h"
#include "ComponentRegistry.h"

int main() {
    SceneManager& sceneManager = SceneManager::GetInstance();

    Scene& scene = sceneManager.CreateScene("Escena 1");

    GameObject& cubo = scene.CreateGameObject("Cubo");
    GameObject& circulo = scene.CreateGameObject("Circulo");

    for (const auto& obj : scene.GetGameObjects()) {
        std::cout << *obj << std::endl;
    }

    cubo.AddChild(circulo);

    std::cout << "-------------------------------" << std::endl;

    for (const auto& obj : scene.GetGameObjects()) {
        std::cout << *obj << std::endl;
    }
}
