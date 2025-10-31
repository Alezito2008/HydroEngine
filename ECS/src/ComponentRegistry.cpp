#include "ComponentRegistry.h"
#include "Component.h"
#include <cassert>

#ifdef _WIN32
#include <windows.h>

#define PLUGIN_EXT ".dll"
#elif __linux__
#include <dlfcn.h>

#define PLUGIN_EXT ".so"
#endif

void ComponentRegistry::Register(const std::string &name, ComponentRegistry::CreatorFunc creator)
{
    auto found = s_Creators.find(name);
    if (found != s_Creators.end()) {
        std::cerr << "Component already registered: " << name << std::endl;
        assert(false && "The component is already registered");
    }
    s_Creators[name] = creator;
}

std::unique_ptr<Component> ComponentRegistry::Create(const std::string& name, GameObject* owner)
{
    auto it = s_Creators.find(name);
    assert(it != s_Creators.end() && "The component isn't registered");
    return it->second(owner);
}

const std::unordered_map<std::string, ComponentRegistry::CreatorFunc> &ComponentRegistry::GetComponents()
{
    return s_Creators;
}

bool ComponentRegistry::LoadLibrary(const std::string &path, bool clear /*= true*/)
{
    if (clear) Clear();

    using RegisterComponentsFunc = void(*)();
    static void* handle = nullptr;

    #ifdef _WIN32
    // TODO: Handle para windows
    #elif __linux__
    if (handle) {
        dlclose(handle);
        handle = nullptr;
    }
    handle = dlopen(path.c_str(), RTLD_NOW | RTLD_LOCAL | RTLD_DEEPBIND);
    if (!handle) {
        std::cout << "[ERROR] Cannot load library: " << path << "\n" << dlerror() << std::endl;
        return false;
    }
    
    dlerror();

    #endif

    return true;
}
