#include "Register.h"

std::unordered_map<std::string, ComponentRegistry::CreatorFunc> components;

extern "C" void RegisterComponents()  {
    for (auto& [name, creator] : components) {
        ComponentRegistry::Register(name, creator);
    }
}