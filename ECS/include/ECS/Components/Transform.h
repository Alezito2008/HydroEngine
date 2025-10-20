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