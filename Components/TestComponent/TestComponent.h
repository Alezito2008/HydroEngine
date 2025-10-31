#pragma once
#include "Component.h"
#include "Register.h"

class TestComponent : public Component {
public:
    int vida = 100;

    TestComponent(GameObject* owner) : Component(owner) {
        SERIALIZED_FIELD(vida);
    }

    inline const std::string& GetName() const override {
        static const std::string name = "TestComponent";
        return name; 
    };
};

class TestComponent3 : public Component {
public:
    int velocidad = 100;

    TestComponent3(GameObject* owner) : Component(owner) {
        SERIALIZED_FIELD(velocidad);
    }

    inline const std::string& GetName() const override {
        static const std::string name = "TestComponent pero 2";
        return name; 
    };
};
