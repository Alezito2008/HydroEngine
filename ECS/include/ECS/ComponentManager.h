#pragma once

#include <unordered_map>
#include <vector>
#include <memory>
#include <typeindex>

#include "GameObject.h"
#include "Component.h"

class Component;

class ComponentManager {
public:
    static ComponentManager& GetInstance() {
        static ComponentManager componentManager;
        return componentManager;
    }

    void RegisterComponent(GameObject* owner, Component* component) {
        if (!owner || !component) return;

        ownerComponentList[owner].push_back(component);
        auto& vec = typeMap[typeid(*component)];
        vec.push_back(component);
    }

    template <typename T>
    T* CreateComponent(GameObject* owner) {
        static_assert(std::is_base_of<Component, T>::value, "T must inherit from Component class");
        
        std::unique_ptr<T> comp = std::make_unique<T>(owner);
        T* raw = comp.get();

        owner->AddComponent(std::move(comp));

        return raw;
    };

    const std::vector<Component*>& GetComponents(GameObject* owner) const {
        static const std::vector<Component*> empty;
        auto it = ownerComponentList.find(owner);
        if (it != ownerComponentList.end()) return it->second;
        return empty;
    }

    template <typename T>
    T* FindComponent(GameObject* owner) {
        static_assert(std::is_base_of<Component, T>::value, "T must inherit from Component class");

        auto it = ownerComponentList.find(owner);
        if (it != ownerComponentList.end()) {
            for (auto comp : it->second) {
                if (auto casted = dynamic_cast<T*>(comp)) return casted;
            }
        }
        return nullptr;
    };

    template <typename T>
    bool RemoveComponent(GameObject* owner) {
        static_assert(std::is_base_of<Component, T>::value, "T must inherit from Component class");

        auto itOwner = ownerComponentList.find(owner);
        if (itOwner == ownerComponentList.end()) {
            // No se enontró el owner
            return false;
        }

        auto& componentsOfOwner = itOwner->second;

        // Chequear que el tipo de componente se haya usado
        auto itType = typeMap.find(typeid(T));
        if (itType == typeMap.end()) {
            // No hay component1es de este tipo
            return false;
        }

        auto& componentsOfType = itType->second;

        for (auto ownerIt = componentsOfOwner.begin(); ownerIt != componentsOfOwner.end(); ++ownerIt) {
            T* casted = dynamic_cast<T*>(*ownerIt);
            if (!casted) continue;            

            componentsOfType.erase(
                std::remove(componentsOfType.begin(), componentsOfType.end(), casted),
                componentsOfType.end()
            );

            componentsOfOwner.erase(
                std::remove(componentsOfOwner.begin(), componentsOfOwner.end(), casted),
                componentsOfOwner.end()
            );

            return true;
        }

        return false;
    }

private:
    ComponentManager() = default;

    // Agrupa componentes del mismo tipo
    std::unordered_map<std::type_index, std::vector<Component*>> typeMap;
    // Agrupa componentes por GameObject
    std::unordered_map<GameObject*, std::vector<Component*>> ownerComponentList;
};