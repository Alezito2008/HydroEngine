#pragma once

#include <memory>

#include <glm/glm.hpp>

#include "graphics/Renderer.h"
#include "Camera.h"
#include "input/InputManager.h"

struct GLFWwindow;
class Shader;
class Model;

class DemoScene {
public:
    DemoScene();
    ~DemoScene();

    void Initialize(GLFWwindow* window, unsigned int width, unsigned int height);
    void Resize(unsigned int width, unsigned int height);
    void Update(float deltaTime, bool allowInput);

    inline unsigned int GetViewportWidth() const { return m_width; }
    inline unsigned int GetViewportHeight() const { return m_height; }

private:
    void UpdateMouseInput(bool allowInput);
    void ProcessMovement(float deltaTime, bool allowInput);
    void UpdateProjection();

    Renderer m_renderer;
    std::unique_ptr<InputManager> m_inputManager;
    Camera m_camera;
    std::unique_ptr<Shader> m_shader;
    std::unique_ptr<Model> m_model;

    glm::mat4 m_projection{};
    glm::vec3 m_lightPos{2.0f, 2.0f, 2.0f};
    glm::vec3 m_lightColor{1.0f, 1.0f, 1.0f};

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

    bool m_initialized{false};
};
