#include "Component.h"

unsigned int Component::m_NextID = 0;

Component::Component(GameObject* owner) {
    m_ID = m_NextID++;
    m_Owner = owner;
}

Component::~Component()
{
    for (auto* field : m_SerializedFields) {
        delete field;
    }
}

void Component::Start() {};
void Component::Update() {};
void Component::OnDestroy() {};

const std::string& Component::GetName() const {
    static const std::string defaultName = "Component";
    return defaultName;
}

void Component::Destroy() {
    OnDestroy();
}
