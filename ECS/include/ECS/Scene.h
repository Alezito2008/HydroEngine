#pragma once

#include <memory>
#include <vector>

#include "GameObject.h"

class Scene {
public:
    Scene(const std::string& name = "");
    ~Scene() = default;

    GameObject& CreateGameObject(const std::string& name) {
        auto gameObject = std::make_unique<GameObject>(name);
        GameObject& ref = *gameObject;
        m_GameObjects.push_back(std::move(gameObject));
        return ref;
    }

    inline const std::string& GetName() const {
        return m_Name;
    }

    inline std::vector<std::unique_ptr<GameObject>>& GetGameObjects() {
        return m_GameObjects;
    }

    inline const std::vector<std::unique_ptr<GameObject>>& GetGameObjects() const {
        return m_GameObjects;
    }
    
private:
    static unsigned int s_NextID;
    unsigned int m_ID;
    std::string m_Name;
    std::vector<std::unique_ptr<GameObject>> m_GameObjects;
};
