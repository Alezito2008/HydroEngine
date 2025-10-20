#include "GameObject.h"
#include "GameObject.inl"
#include "Components/Transform.h"
#include "ComponentManager.h"

unsigned int GameObject::s_NextID = 0;

GameObject::GameObject(const std::string& name)
    : transform(std::make_unique<Transform>(this)) {
    m_ID = ++s_NextID;

    if (name.empty()) {
        m_Name = "GameObject_" + std::to_string(m_ID);
    } else {
        m_Name = name;
    }
}

GameObject *GameObject::FindChild(const std::string &name) const
{
    for (const auto& child : m_Childs) {
        if (child->GetName() == name) {
            return child;
        }
    }
    return nullptr;
}

GameObject *GameObject::FindChild(const unsigned int index) const
{
    if (index < m_Childs.size()) {
        return m_Childs[index];
    }
    return nullptr;
}

void GameObject::AddChild(GameObject &child)
{
    // Evitar ciclos
    GameObject* ancestor = this;
    while (ancestor) {
        if (ancestor == &child) throw std::runtime_error(
            "Cycle detected: trying to add '" + child.m_Name +
            "' as a child of '" + m_Name
        );
        ancestor = ancestor->m_Parent;
    }

    // Eliminarse de la lista de childs del parent anterior
    if (GameObject* parent = child.m_Parent) {
        auto& siblings = parent->m_Childs;
        siblings.erase(
            std::remove(siblings.begin(), siblings.end(), &child),
            siblings.end()
        );
    }

    child.m_Parent = this;
    m_Childs.push_back(&child);
}

const std::vector<Component*>& GameObject::GetComponents()
{
    return ComponentManager::GetInstance().GetComponents(this);
}

Component *GameObject::AddComponent(std::unique_ptr<Component> component)
{
    Component* raw = component.get();
    m_Components.push_back(std::move(component));
    ComponentManager::GetInstance().RegisterComponent(this, raw);
    return raw;
}

std::ostream &operator<<(std::ostream &os, const GameObject &obj)
{
    os << "Name: " << obj.GetName() <<
        "\nID: " << obj.GetID() <<
        "\nParent: " << (obj.GetParent() ? obj.GetParent()->GetName() : "null") <<
        "\nnChilds: " << obj.GetChilds().size() << std::endl;
    return os;
}
