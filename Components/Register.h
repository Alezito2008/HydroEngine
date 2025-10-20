#pragma once
#include "ComponentRegistry.h"
#include "Component.h"
#include "GameObject.h"
#include <unordered_map>
#include <memory>

extern std::unordered_map<std::string, ComponentRegistry::CreatorFunc> components;

#define REGISTER_COMPONENT(COMP) \
namespace { \
    inline bool _register_##COMP = []() { \
        AddComponentToRegister(#COMP, [](GameObject* owner) { \
            return std::make_unique<COMP>(owner); \
        }); \
        return true; \
    }(); \
}

inline void AddComponentToRegister(const std::string& name, ComponentRegistry::CreatorFunc creator) {
    components[name] = creator;
}

extern "C" {
    void RegisterComponents();
}