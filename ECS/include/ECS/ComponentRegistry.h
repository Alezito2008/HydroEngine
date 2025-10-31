#pragma once
#include "GameObject.h"
#include <functional>
#include <memory>
#include <unordered_map>
#include <string>

#define REGISTER_COMPONENT(COMP) \
namespace { \
    struct RegisterComponent_##COMP { \
        RegisterComponent_##COMP() { \
            std::cout << "Registrando componente " << #COMP << std::endl; \
            ComponentRegistry::Register(#COMP, [](GameObject* owner) { \
                return std::make_unique<COMP>(owner); \
            }); \
        } \
    }; \
    RegisterComponent_##COMP s_##COMP; \
}

class ComponentRegistry {
public:
    using CreatorFunc = std::function<std::unique_ptr<Component>(GameObject* owner)>;

    static void Register(const std::string& name, CreatorFunc creator);
    static std::unique_ptr<Component> Create(const std::string& name, GameObject* owner);
    static const std::unordered_map<std::string,CreatorFunc>& GetComponents();
    static bool LoadLibrary(const std::string& path, bool clear = true);
    inline static void Clear() { s_Creators.clear(); }

private:
    inline static std::unordered_map<std::string,CreatorFunc> s_Creators;
};
