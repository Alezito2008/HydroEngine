#include "ComponentManager.h"

template <typename T>
inline T *GameObject::FindComponent()
{
    T* comp = ComponentManager::GetInstance().FindComponent<T>(this);
    return comp;
}

template <typename T>
inline bool GameObject::RemoveComponent()
{
    static_assert(std::is_base_of<Component, T>::value, "T must inherit from Component class");

    if (!ComponentManager::GetInstance().RemoveComponent<T>(this)) return false;

    for (auto& componentsIt = m_Components.begin(); componentsIt != m_Components.end(); ++componentsIt) {
        T* casted = dynamic_cast<T*>(componentsIt->get());
        if (!casted) continue;
        m_Components.erase(
            std::remove(m_Components.begin(), m_Components.end(), casted),
            m_Components.end()
        );

        return true;
    }

    return false;
}

template <typename T>
inline T* GameObject::AddComponent()
{
    T* comp = ComponentManager::GetInstance().CreateComponent<T>(this);
    return comp;
}
