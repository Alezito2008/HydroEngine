#include "DemoScene.h"

#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/constants.hpp>

#include <algorithm>
#include <cmath>
#include <cstdio>

#include "input/InputManager.h"
#include "graphics/Shader.h"
#include "graphics/Model.h"

DemoScene::DemoScene()
    : m_rng(std::random_device{}())
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

    m_shader = std::make_unique<Shader>("res/shaders/model.vert", "res/shaders/model.frag");
    m_model = std::make_unique<Model>("res/models/backpack/backpack.obj");
    m_collectibleModel = std::make_unique<Model>("res/models/diamond/Diamond3D.fbx");

    m_shader->Bind();
    m_shader->setBool("useSolidColor", false);
    m_shader->setVec3("solidColor", glm::vec3(0.0f));
    m_shader->setFloat("solidSpecularStrength", 0.6f);

    UpdateProjection();

    ResetGame();

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
    UpdateGameplay(deltaTime);

    glEnable(GL_DEPTH_TEST);
    glViewport(0, 0, static_cast<GLsizei>(m_width), static_cast<GLsizei>(m_height));

    glClearColor(0.1f, 0.1f, 0.12f, 1.0f);
    m_renderer.Clear();

    glm::mat4 view = m_camera.GetView();

    m_shader->Bind();
    m_shader->setBool("useSolidColor", false);
    m_shader->setVec3("viewPos", m_camera.GetPosition());
    m_shader->setVec3("lightPos", m_lightPos);
    m_shader->setVec3("lightColor", m_lighting.color);
    m_shader->setFloat("lightIntensity", std::max(0.0f, m_lighting.intensity));
    m_shader->setFloat("ambientStrength", std::clamp(m_lighting.ambientStrength, 0.0f, 1.0f));
    m_shader->setFloat("shininess", std::max(1.0f, m_lighting.shininess));

    glm::mat4 model(1.0f);
    model = glm::translate(model, glm::vec3(0.0f, -1.5f, 0.0f));
    model = glm::scale(model, glm::vec3(1.0f));
    model = glm::rotate(model, glm::radians(180.0f), glm::vec3(1.0f, 0.0f, 0.0f));

    m_shader->setMVP(model, view, m_projection);
    m_model->Draw(*m_shader, m_renderer);

    RenderCollectibles(view);
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

    deltaX *= -m_mouseSensitivity;
    deltaY *= -m_mouseSensitivity;

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
    if (!m_inputManager) {
        return;
    }

    if (!allowInput) {
        m_resetHeld = false;
        return;
    }

    glm::vec3 direction(0.0f);

    float velocity = m_cameraSpeed * deltaTime;

    if (m_inputManager->IsKeyPressed(KeyCode::KEY_LEFT_SHIFT)) {
        velocity *= 1.6f;
    }

    if (m_inputManager->IsKeyPressed(KeyCode::KEY_W)) direction += m_camera.GetFront();
    if (m_inputManager->IsKeyPressed(KeyCode::KEY_S)) direction -= m_camera.GetFront();
    if (m_inputManager->IsKeyPressed(KeyCode::KEY_A)) direction -= m_camera.GetRight();
    if (m_inputManager->IsKeyPressed(KeyCode::KEY_D)) direction += m_camera.GetRight();

    if (glm::length(direction) > 0.0f) {
        direction = glm::normalize(direction) * velocity;
        m_camera.Move(direction);
    }

    bool resetPressed = m_inputManager->IsKeyPressed(KeyCode::KEY_R);
    if (resetPressed && !m_resetHeld) {
        ResetGame();
        m_resetHeld = true;
    } else if (!resetPressed) {
        m_resetHeld = false;
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

void DemoScene::UpdateGameplay(float deltaTime)
{
    m_elapsedTime += deltaTime;
    m_stats.elapsed = m_elapsedTime;

    if (m_waveMessageTimer > 0.0f) {
        m_waveMessageTimer -= deltaTime;
        if (m_waveMessageTimer <= 0.0f) {
            m_waveMessageTimer = 0.0f;
            m_waveMessage.clear();
        }
    }

    if (m_collectibles.empty()) {
        return;
    }

    glm::vec3 cameraPos = m_camera.GetPosition();
    int collectedCount = 0;

    for (auto& collectible : m_collectibles) {
        if (!collectible.collected) {
            collectible.rotationAngle += deltaTime * collectible.rotationSpeed;
            if (collectible.rotationAngle > glm::two_pi<float>()) {
                collectible.rotationAngle = std::fmod(collectible.rotationAngle, glm::two_pi<float>());
            }

            float distance = glm::length(cameraPos - collectible.position);
            if (distance <= m_collectiblePickupRadius) {
                collectible.collected = true;
                m_stats.score += 10;
            }
        }

        if (collectible.collected) {
            ++collectedCount;
        }
    }

    m_stats.total = static_cast<int>(m_collectibles.size());
    m_stats.collected = collectedCount;

    if (m_stats.total > 0 && collectedCount == m_stats.total) {
        int clearedWave = m_stats.wave;
        std::size_t nextCount = std::min<std::size_t>(m_initialCollectibleCount + static_cast<std::size_t>(clearedWave) * 2, m_maxCollectibleCount);
        SpawnCollectibles(nextCount);
        m_stats.score += 25 + clearedWave * 5;

        char buffer[96];
        std::snprintf(buffer, sizeof(buffer), "Wave %d cleared! Wave %d ready!", clearedWave, m_stats.wave);
        m_waveMessage = buffer;
        m_waveMessageTimer = m_messageDuration;
    }
}

void DemoScene::RenderCollectibles(const glm::mat4& view)
{
    if (!m_shader || !m_collectibleModel || m_collectibles.empty()) {
        return;
    }

    const glm::vec3 collectibleColor(0.35f, 0.75f, 1.0f);
    m_shader->setBool("useSolidColor", true);
    m_shader->setVec3("solidColor", collectibleColor);
    m_shader->setFloat("solidSpecularStrength", 0.85f);

    for (const auto& collectible : m_collectibles) {
        if (collectible.collected) {
            continue;
        }

        glm::vec3 position = collectible.position;
        float bobOffset = std::sin((m_elapsedTime + collectible.bobbingPhase) * collectible.bobbingSpeed) * 0.35f;
        position.y += bobOffset;

        glm::mat4 model(1.0f);
        model = glm::translate(model, position);
        model = glm::rotate(model, glm::radians(180.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::rotate(model, collectible.rotationAngle, collectible.rotationAxis);
        model = glm::scale(model, glm::vec3(m_collectibleScale));

    m_shader->setMVP(model, view, m_projection);
    m_collectibleModel->Draw(*m_shader, m_renderer);
    }

    m_shader->setBool("useSolidColor", false);
    m_shader->setFloat("solidSpecularStrength", 0.6f);
}

void DemoScene::SpawnCollectibles(std::size_t count)
{
    if (count == 0) {
        count = 1;
    }

    count = std::min<std::size_t>(count, m_maxCollectibleCount);

    std::uniform_real_distribution<float> distX(-6.5f, 6.5f);
    std::uniform_real_distribution<float> distZ(-6.5f, 6.5f);
    std::uniform_real_distribution<float> distY(-1.0f, 2.0f);
    std::uniform_real_distribution<float> bobPhase(0.0f, glm::two_pi<float>());
    std::uniform_real_distribution<float> bobSpeed(1.0f, 2.2f);
    std::uniform_real_distribution<float> rotationSpeed(0.8f, 1.8f);

    const float minDistanceFromCenter = 1.5f;
    const float minDistanceBetween = 1.25f;

    m_collectibles.clear();
    m_collectibles.reserve(count);

    for (std::size_t i = 0; i < count; ++i) {
        Collectible collectible;
        bool placed = false;
        int attempts = 0;

        while (!placed && attempts < 32) {
            glm::vec3 candidate(distX(m_rng), distY(m_rng), distZ(m_rng));
            glm::vec2 candidateXZ(candidate.x, candidate.z);
            float centerDistance = glm::length(candidateXZ);

            bool tooCloseToCenter = centerDistance < minDistanceFromCenter;
            bool overlaps = false;

            if (!tooCloseToCenter) {
                for (const auto& existing : m_collectibles) {
                    if (glm::length(existing.position - candidate) < minDistanceBetween) {
                        overlaps = true;
                        break;
                    }
                }
            }

            if (!tooCloseToCenter && !overlaps) {
                collectible.position = candidate;
                placed = true;
            }

            ++attempts;
        }

        if (!placed) {
            collectible.position = glm::vec3(distX(m_rng), distY(m_rng), distZ(m_rng));
        }

        collectible.rotationAxis = glm::vec3(0.0f, 1.0f, 0.0f);
        collectible.rotationSpeed = rotationSpeed(m_rng);
        collectible.bobbingPhase = bobPhase(m_rng);
        collectible.bobbingSpeed = bobSpeed(m_rng);
        collectible.rotationAngle = 0.0f;
        collectible.collected = false;

        m_collectibles.emplace_back(collectible);
    }

    m_stats.total = static_cast<int>(m_collectibles.size());
    m_stats.collected = 0;
    m_stats.wave += 1;
}

void DemoScene::ResetGame()
{
    m_camera.SetPosition(glm::vec3(0.0f, 0.0f, 3.0f));
    m_camera.SetFront(glm::vec3(0.0f, 0.0f, -1.0f));

    m_pitch = 0.0f;
    m_yaw = -90.0f;
    m_firstMouse = true;

    m_resetHeld = false;

    m_elapsedTime = 0.0f;
    m_stats = GameplayStats{};
    m_waveMessage.clear();
    m_waveMessageTimer = 0.0f;

    SpawnCollectibles(m_initialCollectibleCount);

    char buffer[96];
    std::snprintf(buffer, sizeof(buffer), "Wave %d ready! Collect them all.", m_stats.wave);
    m_waveMessage = buffer;
    m_waveMessageTimer = m_messageDuration;
}
