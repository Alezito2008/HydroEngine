#pragma once

#include "Scene.h"

class SceneManager {
public:
    static SceneManager& GetInstance() {
        static SceneManager sceneManager;
        return sceneManager;
    }

    Scene& CreateScene(const std::string& name = "");
    void SetActiveScene(const std::string& name);

    Scene& GetActiveScene();
    std::vector<std::unique_ptr<Scene>>& GetScenes();
    Scene* FindScene(const std::string& name);
    Scene& GetOrCreateScene(const std::string& name);
private:
    std::vector<std::unique_ptr<Scene>> m_Scenes;
    int m_ActiveSceneIndex = -1;
};
