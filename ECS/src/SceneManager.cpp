#include "SceneManager.h"
#include <cassert>

Scene& SceneManager::CreateScene(const std::string& name)
{
    auto scene = std::make_unique<Scene>(name);
    Scene& ref = *scene;
    m_Scenes.push_back(std::move(scene));
    return ref;
}

void SceneManager::SetActiveScene(const std::string& name)
{
    for (size_t i = 0; i < m_Scenes.size(); i++) {
        if (m_Scenes[i]->GetName() == name) {
            m_ActiveSceneIndex = i;
            return;
        }
    }
    assert(false && "Scene not found!");
}

Scene &SceneManager::GetActiveScene()
{
    assert(m_ActiveSceneIndex >= 0 && m_ActiveSceneIndex < m_Scenes.size() && "Scene not found!");
    return *m_Scenes[m_ActiveSceneIndex];
}

std::vector<std::unique_ptr<Scene>> &SceneManager::GetScenes()
{
    return m_Scenes;
}
