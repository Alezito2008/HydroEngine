#include "DemoScene.h"

#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>

#include <algorithm>
#include <cmath>

#include "input/InputManager.h"
#include "graphics/Shader.h"
#include "graphics/Model.h"

DemoScene::DemoScene()
{
}

DemoScene::~DemoScene() = default;

void DemoScene::Initialize(GLFWwindow* window, unsigned int width, unsigned int height)
{
    m_window = window;
    m_width = width;
    m_height = height;

    m_inputManager = std::make_unique<InputManager>(m_window);

    glEnable(GL_DEPTH_TEST);

    m_camera.SetPosition(glm::vec3(0.0f, 0.0f, 3.0f));
    m_camera.SetFront(glm::vec3(0.0f, 0.0f, -1.0f));

    m_shader = std::make_unique<Shader>("res/shaders/model.vert", "res/shaders/model.frag");
    m_model = std::make_unique<Model>("res/models/backpack/backpack.obj");

    UpdateProjection();

    double xpos, ypos;
    glfwGetCursorPos(m_window, &xpos, &ypos);
    m_lastX = xpos;
    m_lastY = ypos;
    m_firstMouse = true;

    m_initialized = true;
}

void DemoScene::Resize(unsigned int width, unsigned int height)
{
    if (width == 0 || height == 0) {
        return;
    }

    if (m_width == width && m_height == height) {
        return;
    }

    m_width = width;
    m_height = height;
    UpdateProjection();
}

void DemoScene::Update(float deltaTime, bool allowInput)
{
    if (!m_initialized || m_window == nullptr || m_width == 0 || m_height == 0 || !m_shader || !m_model) {
        return;
    }

    UpdateMouseInput(allowInput);
    ProcessMovement(deltaTime, allowInput);

    glEnable(GL_DEPTH_TEST);
    glViewport(0, 0, static_cast<GLsizei>(m_width), static_cast<GLsizei>(m_height));

    glClearColor(0.1f, 0.1f, 0.12f, 1.0f);
    m_renderer.Clear();

    glm::mat4 view = m_camera.GetView();

    m_shader->Bind();
    m_shader->setVec3("viewPos", m_camera.GetPosition());
    m_shader->setVec3("lightPos", m_lightPos);
    m_shader->setVec3("lightColor", m_lighting.color);
    m_shader->setFloat("lightIntensity", std::max(0.0f, m_lighting.intensity));
    m_shader->setFloat("ambientStrength", std::clamp(m_lighting.ambientStrength, 0.0f, 1.0f));
    m_shader->setFloat("shininess", std::max(1.0f, m_lighting.shininess));

    glm::mat4 model(1.0f);
    model = glm::translate(model, glm::vec3(0.0f, -1.5f, 0.0f));
    model = glm::scale(model, glm::vec3(1.0f));

    m_shader->setMVP(model, view, m_projection);
    m_model->Draw(*m_shader, m_renderer);
}

void DemoScene::UpdateMouseInput(bool allowInput)
{
    if (!allowInput) {
        m_firstMouse = true;
        return;
    }

    double xpos, ypos;
    glfwGetCursorPos(m_window, &xpos, &ypos);

    if (m_firstMouse) {
        m_lastX = xpos;
        m_lastY = ypos;
        m_firstMouse = false;
    }

    float deltaX = static_cast<float>(xpos - m_lastX);
    float deltaY = static_cast<float>(m_lastY - ypos);

    m_lastX = xpos;
    m_lastY = ypos;

    deltaX *= m_mouseSensitivity;
    deltaY *= m_mouseSensitivity;

    m_yaw += deltaX;
    m_pitch += deltaY;

    if (m_pitch > 89.0f) m_pitch = 89.0f;
    if (m_pitch < -89.0f) m_pitch = -89.0f;

    glm::vec3 front;
    front.x = cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
    front.y = sin(glm::radians(m_pitch));
    front.z = sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));

    m_camera.SetFront(glm::normalize(front));
}

void DemoScene::ProcessMovement(float deltaTime, bool allowInput)
{
    if (!allowInput || !m_inputManager) {
        return;
    }

    glm::vec3 direction(0.0f);

    float velocity = m_cameraSpeed * deltaTime;

    if (m_inputManager->IsKeyPressed(KeyCode::KEY_W)) direction += m_camera.GetFront();
    if (m_inputManager->IsKeyPressed(KeyCode::KEY_S)) direction -= m_camera.GetFront();
    if (m_inputManager->IsKeyPressed(KeyCode::KEY_A)) direction -= m_camera.GetRight();
    if (m_inputManager->IsKeyPressed(KeyCode::KEY_D)) direction += m_camera.GetRight();

    if (glm::length(direction) > 0.0f) {
        direction = glm::normalize(direction) * velocity;
        m_camera.Move(direction);
    }
}

void DemoScene::UpdateProjection()
{
    if (m_height == 0) {
        return;
    }

    float aspect = static_cast<float>(m_width) / static_cast<float>(m_height);
    m_projection = glm::perspective(glm::radians(75.0f), aspect, 0.1f, 100.0f);
}
