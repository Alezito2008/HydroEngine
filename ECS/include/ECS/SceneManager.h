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
private:
    std::vector<std::unique_ptr<Scene>> m_Scenes;
    int m_ActiveSceneIndex = -1;
};
