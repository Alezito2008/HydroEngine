#pragma once

#include <memory>
#include <random>
#include <string>
#include <vector>

#include <glm/glm.hpp>

#include "graphics/Renderer.h"
#include "Camera.h"
#include "input/InputManager.h"

struct GLFWwindow;
class Shader;
class Model;

class DemoScene {
public:
    struct LightingSettings {
        glm::vec3 color{1.0f, 1.0f, 1.0f};
        float intensity{1.0f};
        float ambientStrength{0.2f};
        float shininess{32.0f};
    };

    struct GameplayStats {
        int score{0};
        int collected{0};
        int total{0};
        int wave{0};
        float elapsed{0.0f};
    };

    DemoScene();
    ~DemoScene();

    void Initialize(GLFWwindow* window, unsigned int width, unsigned int height);
    void Resize(unsigned int width, unsigned int height);
    void Update(float deltaTime, bool allowInput);

    LightingSettings& GetLightingSettings() { return m_lighting; }
    const LightingSettings& GetLightingSettings() const { return m_lighting; }

    const GameplayStats& GetStats() const { return m_stats; }
    const std::string& GetWaveMessage() const { return m_waveMessage; }

    inline unsigned int GetViewportWidth() const { return m_width; }
    inline unsigned int GetViewportHeight() const { return m_height; }

private:
    void UpdateMouseInput(bool allowInput);
    void ProcessMovement(float deltaTime, bool allowInput);
    void UpdateProjection();
    void UpdateGameplay(float deltaTime);
    void RenderCollectibles(const glm::mat4& view);
    void SpawnCollectibles(std::size_t count);
    void ResetGame();

    struct Collectible {
        glm::vec3 position{0.0f};
        glm::vec3 rotationAxis{0.0f, 1.0f, 0.0f};
        float rotationAngle{0.0f};
        float rotationSpeed{1.0f};
        float bobbingPhase{0.0f};
        float bobbingSpeed{1.0f};
        bool collected{false};
    };

    Renderer m_renderer;
    std::unique_ptr<InputManager> m_inputManager;
    Camera m_camera;
    std::unique_ptr<Shader> m_shader;
    std::unique_ptr<Model> m_model;
    std::unique_ptr<Model> m_collectibleModel;

    glm::mat4 m_projection{};
    glm::vec3 m_lightPos{2.0f, 2.0f, 2.0f};
    LightingSettings m_lighting{};
    GameplayStats m_stats{};

    std::vector<Collectible> m_collectibles;

    std::mt19937 m_rng;

    float m_collectibleScale{0.6f};
    float m_collectiblePickupRadius{1.5f};
    std::size_t m_initialCollectibleCount{4};
    std::size_t m_maxCollectibleCount{18};
    float m_messageDuration{3.5f};
    std::string m_waveMessage;
    float m_waveMessageTimer{0.0f};

    float m_elapsedTime{0.0f};

    GLFWwindow* m_window{nullptr};

    unsigned int m_width{0};
    unsigned int m_height{0};

    float m_pitch{0.0f};
    float m_yaw{-90.0f};
    double m_lastX{0.0};
    double m_lastY{0.0};
    bool m_firstMouse{true};

    float m_cameraSpeed{5.0f};
    float m_mouseSensitivity{0.1f};
    bool m_resetHeld{false};

    bool m_initialized{false};
};
