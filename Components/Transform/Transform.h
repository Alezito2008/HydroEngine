#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Component.h"

class GameObject;

class Transform : public Component {
public:
    glm::vec3 position;
    glm::vec3 rotation;
    glm::vec3 scale;

    glm::mat4 worldMatrix;

    int x, y, z;

    Transform(GameObject* owner)
        : Component(owner), position(0.0f), rotation(0.0f), scale(0.0f) {
            SerializeField("x", x);
            SerializeField("y", y);
            SerializeField("z", z);
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

    model = glm::rotate(model, rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
    model = glm::rotate(model, rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::rotate(model, rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));

    model = glm::scale(model, scale);

    return model;
}

inline void Transform::UpdateWorldMatrix() {
    GameObject* parent = m_Owner->GetParent();
    if (parent) {
        Transform& parentTransform = *(parent->transform);
        worldMatrix = parentTransform.GetLocalMatrix();
    } else {
        worldMatrix = GetLocalMatrix();
    }
}
