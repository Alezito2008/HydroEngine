#include "DemoScene.h"

#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/constants.hpp>

#include <algorithm>
#include <cmath>
#include <cstdio>
#include <string>
#include <utility>

#include "input/InputManager.h"
#include "graphics/Shader.h"
#include "graphics/Model.h"

#include "SceneManager.h"
#include "Scene.h"
#include "GameObject.h"
#include "GameObject.inl"
#include "Transform/Transform.h"
#include "Collectible/CollectibleComponent.h"

namespace {
const glm::vec3 kDefaultBackpackPosition{0.0f, -1.5f, 0.0f};
const glm::vec3 kDefaultBackpackRotation{180.0f, 0.0f, 0.0f};
const glm::vec3 kDefaultBackpackScale{1.0f};
constexpr const char* kSceneName = "Demo Scene";
constexpr const char* kWorldRootName = "World";
constexpr const char* kBackpackName = "Backpack Display";

GameObject* FindRecursive(GameObject* object, const std::string& name) {
	if (!object) {
		return nullptr;
	}
	if (object->GetName() == name) {
		return object;
	}
	for (GameObject* child : object->GetChilds()) {
		if (GameObject* found = FindRecursive(child, name)) {
			return found;
		}
	}
	return nullptr;
}
} // namespace

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
	SetupECSScene();
	ResetGame();

	double xpos, ypos;
	glfwGetCursorPos(m_window, &xpos, &ypos);
	m_lastX = xpos;
	m_lastY = ypos;
	m_firstMouse = true;

	m_initialized = true;
}

void DemoScene::Reload()
{
	if (!m_initialized || m_window == nullptr) {
		return;
	}

	glEnable(GL_DEPTH_TEST);

	m_inputManager = std::make_unique<InputManager>(m_window);

	m_shader = std::make_unique<Shader>("res/shaders/model.vert", "res/shaders/model.frag");
	m_model = std::make_unique<Model>("res/models/backpack/backpack.obj");
	m_collectibleModel = std::make_unique<Model>("res/models/diamond/Diamond3D.fbx");

	m_shader->Bind();
	m_shader->setBool("useSolidColor", false);
	m_shader->setVec3("solidColor", glm::vec3(0.0f));
	m_shader->setFloat("solidSpecularStrength", 0.6f);

	UpdateProjection();
	m_sceneReady = false;
	ResetGame();
}

void DemoScene::ResetGameplay()
{
	if (!m_initialized) {
		return;
	}

	ResetGame();
}

void DemoScene::SetCollectibleCollectedCallback(CollectibleCollectedCallback callback)
{
	m_collectibleCollectedCallback = std::move(callback);
}

void DemoScene::SetCollectibleLogCallback(CollectibleLogCallback callback)
{
	m_collectibleLogCallback = std::move(callback);
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

	glm::mat4 modelMatrix(1.0f);
	if (m_backpackTransform) {
		modelMatrix = BuildModelMatrix(*m_backpackTransform, 0.0f, 0.0f);
	} else {
		modelMatrix = glm::translate(modelMatrix, kDefaultBackpackPosition);
		modelMatrix = glm::rotate(modelMatrix, glm::radians(kDefaultBackpackRotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
		modelMatrix = glm::scale(modelMatrix, kDefaultBackpackScale);
	}

	m_shader->setMVP(modelMatrix, view, m_projection);
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
	front.x = std::cos(glm::radians(m_yaw)) * std::cos(glm::radians(m_pitch));
	front.y = std::sin(glm::radians(m_pitch));
	front.z = std::sin(glm::radians(m_yaw)) * std::cos(glm::radians(m_pitch));

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
	if (!m_sceneReady) {
		return;
	}

	m_elapsedTime += deltaTime;
	m_stats.elapsed = m_elapsedTime;

	if (m_waveMessageTimer > 0.0f) {
		m_waveMessageTimer -= deltaTime;
		if (m_waveMessageTimer <= 0.0f) {
			m_waveMessageTimer = 0.0f;
			m_waveMessage.clear();
		}
	}

	if (m_activeCollectibles == 0) {
		return;
	}

	glm::vec3 cameraPos = m_camera.GetPosition();
	int collectedCount = 0;

	for (std::size_t i = 0; i < m_activeCollectibles; ++i) {
		auto& handle = m_collectibles[i];
		if (!handle.object || !handle.transform || !handle.component) {
			continue;
		}

		CollectibleComponent& data = *handle.component;

		bool isActive = handle.object->IsActive();

		if (!isActive) {
			if (!data.collected) {
				data.collected = true;
			}
			++collectedCount;
			continue;
		}

		if (data.collected && isActive) {
			data.collected = false;
		}

		if (data.enableRotation && data.rotationSpeed != 0.0f) {
			data.spinPhase += deltaTime * data.rotationSpeed;
			if (data.spinPhase > glm::two_pi<float>() || data.spinPhase < -glm::two_pi<float>()) {
				data.spinPhase = std::fmod(data.spinPhase, glm::two_pi<float>());
			}
		}

		if (data.enableBobbing) {
			data.bobPhase += deltaTime * data.bobbingSpeed;
			if (data.bobPhase > glm::two_pi<float>() || data.bobPhase < -glm::two_pi<float>()) {
				data.bobPhase = std::fmod(data.bobPhase, glm::two_pi<float>());
			}
			handle.bobbingOffset = std::sin(data.bobPhase) * data.bobbingAmplitude;
		} else {
			handle.bobbingOffset = 0.0f;
		}

		glm::vec3 worldPos = handle.transform->position;
		worldPos.y += handle.bobbingOffset;

		float distance = glm::length(cameraPos - worldPos);
		if (distance <= data.pickupRadius) {
			// Pickup handler
			data.collected = true;
			handle.object->SetActive(false);
			m_stats.score += data.scoreValue;
			++collectedCount;
			if (m_collectibleLogCallback) {
				char message[128];
				std::snprintf(message, sizeof(message), "Diamond collected (+%d). Score: %d | %d/%zu collected", data.scoreValue, m_stats.score, collectedCount, m_activeCollectibles);
				const std::string messageStr(message);
				m_collectibleLogCallback(messageStr);
			}
			if (m_collectibleCollectedCallback) {
				m_collectibleCollectedCallback(data.scoreValue, m_stats.score, collectedCount, static_cast<int>(m_activeCollectibles));
			}
			continue;
		}

		if (data.collected) {
			++collectedCount;
		}
	}

	m_stats.total = static_cast<int>(m_activeCollectibles);
	m_stats.collected = collectedCount;

	if (m_activeCollectibles > 0 && collectedCount == static_cast<int>(m_activeCollectibles)) {
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
	if (!m_shader || !m_collectibleModel || m_activeCollectibles == 0) {
		return;
	}

	const glm::vec3 collectibleColor(0.35f, 0.75f, 1.0f);
	m_shader->setBool("useSolidColor", true);
	m_shader->setVec3("solidColor", collectibleColor);
	m_shader->setFloat("solidSpecularStrength", 0.85f);

	for (std::size_t i = 0; i < m_activeCollectibles; ++i) {
		const auto& handle = m_collectibles[i];
		if (!handle.object || !handle.transform || !handle.component) {
			continue;
		}

		const CollectibleComponent& data = *handle.component;
		if (data.collected || !handle.object->IsActive()) {
			continue;
		}

		glm::mat4 model = BuildModelMatrix(*handle.transform, handle.bobbingOffset, data.enableRotation ? data.spinPhase : 0.0f);

		m_shader->setMVP(model, view, m_projection);
		m_collectibleModel->Draw(*m_shader, m_renderer);
	}

	m_shader->setBool("useSolidColor", false);
	m_shader->setFloat("solidSpecularStrength", 0.6f);
}

void DemoScene::SpawnCollectibles(std::size_t count)
{
	if (!m_sceneReady) {
		SetupECSScene();
	}

	EnsureCollectiblePool();

	if (count == 0) {
		count = 1;
	}

	count = std::min<std::size_t>(count, m_maxCollectibleCount);
	m_activeCollectibles = count;

	std::uniform_real_distribution<float> distX(-6.5f, 6.5f);
	std::uniform_real_distribution<float> distZ(-6.5f, 6.5f);
	std::uniform_real_distribution<float> distY(-1.0f, 2.0f);
	std::uniform_real_distribution<float> bobPhase(0.0f, glm::two_pi<float>());
	std::uniform_real_distribution<float> spinStart(0.0f, glm::two_pi<float>());

	float amplitudeMin = std::min(m_collectibleAmplitudeMin, m_collectibleAmplitudeMax);
	float amplitudeMax = std::max(m_collectibleAmplitudeMin, m_collectibleAmplitudeMax);
	std::uniform_real_distribution<float> amplitudeDist(amplitudeMin, amplitudeMax);

	const float minDistanceFromCenter = 1.5f;
	const float minDistanceBetween = 1.25f;

	std::vector<glm::vec3> placed;
	placed.reserve(count);

	for (std::size_t i = 0; i < m_collectibles.size(); ++i) {
		auto& handle = m_collectibles[i];
		if (!handle.object || !handle.transform || !handle.component) {
			continue;
		}

		CollectibleComponent& data = *handle.component;

		if (i < count) {
			glm::vec3 position{};
			bool placedSuccessfully = false;
			int attempts = 0;
			while (!placedSuccessfully && attempts < 32) {
				glm::vec3 candidate(distX(m_rng), distY(m_rng), distZ(m_rng));
				float centerDistance = glm::length(glm::vec2(candidate.x, candidate.z));

				bool tooCloseToCenter = centerDistance < minDistanceFromCenter;
				bool overlaps = std::any_of(placed.begin(), placed.end(), [&](const glm::vec3& existing) {
					return glm::length(existing - candidate) < minDistanceBetween;
				});

				if (!tooCloseToCenter && !overlaps) {
					position = candidate;
					placedSuccessfully = true;
				}
				++attempts;
			}

			if (!placedSuccessfully) {
				position = glm::vec3(distX(m_rng), distY(m_rng), distZ(m_rng));
			}

			handle.transform->position = position;
			handle.transform->rotation = glm::vec3(180.0f, 0.0f, 0.0f);
			handle.transform->scale = glm::vec3(m_collectibleScale);

			data.bobbingAmplitude = amplitudeDist(m_rng);
			data.ResetRuntime(spinStart(m_rng), bobPhase(m_rng));
			data.collected = false;

			handle.object->SetActive(true);
			handle.bobbingOffset = 0.0f;
			placed.emplace_back(position);
		} else {
			data.collected = true;
			handle.object->SetActive(false);
			handle.bobbingOffset = 0.0f;
		}
	}

	m_stats.total = static_cast<int>(m_activeCollectibles);
	m_stats.collected = 0;
	m_stats.wave += 1;
}

void DemoScene::ResetGame()
{
	SetupECSScene();
	EnsureCollectiblePool();

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
	m_activeCollectibles = 0;

	if (m_backpackTransform) {
		m_backpackTransform->position = kDefaultBackpackPosition;
		m_backpackTransform->rotation = kDefaultBackpackRotation;
		m_backpackTransform->scale = kDefaultBackpackScale;
	}

	for (auto& handle : m_collectibles) {
		if (handle.component) {
			handle.component->ResetRuntime(0.0f, 0.0f);
			handle.component->collected = true;
		}
		if (handle.object) {
			handle.object->SetActive(false);
		}
		handle.bobbingOffset = 0.0f;
	}

	SpawnCollectibles(m_initialCollectibleCount);

	char buffer[96];
	std::snprintf(buffer, sizeof(buffer), "Wave %d ready! Collect them all.", m_stats.wave);
	m_waveMessage = buffer;
	m_waveMessageTimer = m_messageDuration;
}

void DemoScene::SetupECSScene()
{
	if (m_sceneReady) {
		return;
	}

	SceneManager& manager = SceneManager::GetInstance();
	Scene& scene = manager.GetOrCreateScene(kSceneName);
	m_scene = &scene;
	manager.SetActiveScene(kSceneName);

	m_worldRoot = FindSceneObject(kWorldRootName);
	if (!m_worldRoot) {
		m_worldRoot = &scene.CreateGameObject(kWorldRootName);
	}

	m_backpackObject = FindSceneObject(kBackpackName);
	if (!m_backpackObject) {
		m_backpackObject = &scene.CreateGameObject(kBackpackName);
		if (m_worldRoot) {
			m_worldRoot->AddChild(*m_backpackObject);
		}
	} else if (m_worldRoot && m_backpackObject->GetParent() != m_worldRoot) {
		m_worldRoot->AddChild(*m_backpackObject);
	}

	m_backpackTransform = m_backpackObject ? m_backpackObject->transform.get() : nullptr;
	if (m_backpackTransform && m_backpackTransform->scale == glm::vec3(0.0f)) {
		m_backpackTransform->scale = kDefaultBackpackScale;
	}

	EnsureCollectiblePool();

	m_sceneReady = true;
}

void DemoScene::EnsureCollectiblePool()
{
	if (!m_scene) {
		return;
	}

	while (m_collectibles.size() < m_maxCollectibleCount) {
		std::string name = "Collectible " + std::to_string(m_collectibles.size() + 1);
		GameObject& object = m_scene->CreateGameObject(name);
		if (m_worldRoot) {
			m_worldRoot->AddChild(object);
		}

		auto* component = object.AddComponent<CollectibleComponent>();
		Transform* transform = object.transform.get();
		transform->scale = glm::vec3(m_collectibleScale);
		transform->rotation = glm::vec3(180.0f, 0.0f, 0.0f);
		transform->position = glm::vec3(0.0f);
		object.SetActive(false);

		CollectibleHandle handle;
		handle.object = &object;
		handle.transform = transform;
		handle.component = component;
		handle.bobbingOffset = 0.0f;
		m_collectibles.emplace_back(handle);
	}

	for (auto& handle : m_collectibles) {
		if (!handle.object) {
			continue;
		}
		if (!handle.transform) {
			handle.transform = handle.object->transform.get();
		}
		if (!handle.component) {
			handle.component = handle.object->AddComponent<CollectibleComponent>();
		}
		if (m_worldRoot && handle.object->GetParent() != m_worldRoot) {
			m_worldRoot->AddChild(*handle.object);
		}
	}
}

GameObject* DemoScene::FindSceneObject(const std::string& name) const
{
	if (!m_scene) {
		return nullptr;
	}

	for (const auto& obj : m_scene->GetGameObjects()) {
		if (!obj) {
			continue;
		}
		if (GameObject* found = FindRecursive(obj.get(), name)) {
			return found;
		}
	}
	return nullptr;
}

glm::mat4 DemoScene::BuildModelMatrix(const Transform& transform, float bobOffset, float spinAngle) const
{
	glm::mat4 model(1.0f);
	glm::vec3 position = transform.position;
	position.y += bobOffset;

	model = glm::translate(model, position);
	model = glm::rotate(model, glm::radians(transform.rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::rotate(model, glm::radians(transform.rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::rotate(model, glm::radians(transform.rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
	if (spinAngle != 0.0f) {
		model = glm::rotate(model, spinAngle, glm::vec3(0.0f, 1.0f, 0.0f));
	}
	model = glm::scale(model, transform.scale);

	return model;
}
