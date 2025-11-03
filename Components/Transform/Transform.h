#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Component.h"

class GameObject;

class Transform : public Component {
public:
    glm::vec3 position{0.0f};
    glm::vec3 rotation{0.0f};
    glm::vec3 scale{1.0f};

    glm::mat4 worldMatrix{1.0f};

    Transform(GameObject* owner)
        : Component(owner) {
            SerializeField("Position", position);
            SerializeField("Rotation", rotation);
            SerializeField("Scale", scale);
        }

    const std::string& GetName() const override {
        static const std::string name = "Transform";
        return name;
    }

    glm::mat4 GetLocalMatrix() const;

    void UpdateWorldMatrix();

    void Update() override {
        UpdateWorldMatrix();
    }

};

inline glm::mat4 Transform::GetLocalMatrix() const {
    glm::mat4 model(1.0f);
    model = glm::translate(model, position);

    model = glm::rotate(model, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
    model = glm::rotate(model, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::rotate(model, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));

    model = glm::scale(model, scale);

    return model;
}

inline void Transform::UpdateWorldMatrix() {
    GameObject* parent = m_Owner->GetParent();
    glm::mat4 local = GetLocalMatrix();

    if (parent) {
        Transform* parentTransform = parent->transform.get();
        if (parentTransform) {
            parentTransform->UpdateWorldMatrix();
            worldMatrix = parentTransform->worldMatrix * local;
            return;
        }
    }

    worldMatrix = local;
}
