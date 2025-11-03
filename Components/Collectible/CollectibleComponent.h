#pragma once

#include "Component.h"

#include <random>
#include <string>

class CollectibleComponent : public Component {
public:
    float rotationSpeed{1.2f};
    float bobbingAmplitude{0.35f};
    float bobbingSpeed{1.5f};
    float pickupRadius{1.5f};
    int scoreValue{10};
    bool enableRotation{true};
    bool enableBobbing{true};

    float spinPhase{0.0f};
    float bobPhase{0.0f};
    bool collected{false};

    CollectibleComponent(GameObject* owner)
        : Component(owner)
    {
        SERIALIZED_FIELD(rotationSpeed);
        SERIALIZED_FIELD(bobbingAmplitude);
        SERIALIZED_FIELD(bobbingSpeed);
        SERIALIZED_FIELD(pickupRadius);
        SERIALIZED_FIELD(scoreValue);
        SERIALIZED_FIELD(enableRotation);
        SERIALIZED_FIELD(enableBobbing);
    }

    const std::string& GetName() const override {
        static const std::string name = "Collectible";
        return name;
    }

    void ResetRuntime(float spin, float bob)
    {
        spinPhase = spin;
        bobPhase = bob;
        collected = false;
    }
};
