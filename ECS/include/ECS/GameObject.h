#pragma once
#include <vector>
#include <memory>
#include <string>
#include <iostream>
#include <algorithm>

// #include "Transform/Transform.h"
class Transform;
class Component;

class GameObject {
    public:
        GameObject(const std::string& name = "");
        ~GameObject();

        std::unique_ptr<Transform> transform;

        inline unsigned int GetID() const { return m_ID; };
        inline const std::string& GetName() const { return m_Name; }
        inline GameObject* GetParent() const { return m_Parent; }

        inline bool* GetActivePtr() { return &m_IsActive; }
        inline void SetActive(bool active) { m_IsActive = active; }
        inline bool IsActive() const { return m_IsActive; }

        inline std::vector<GameObject*>& GetChilds() { return m_Childs; };
        inline const std::vector<GameObject*>& GetChilds() const { return m_Childs; }

        inline GameObject* FindFirstChild() const { return FindChild(0); }
        GameObject* FindChild(const std::string& name) const;
        GameObject* FindChild(const unsigned int index) const;
        void AddChild(GameObject& child);
        
        const std::vector<Component*>& GetComponents();

        template <typename T> T* FindComponent();
        template <typename T> bool RemoveComponent();
        template <typename T> T* AddComponent(); 
        Component* AddComponent(std::unique_ptr<Component> component);
    private:
        unsigned int m_ID;
        static unsigned int s_NextID;

        bool m_IsActive = true;
        std::string m_Name;

        std::vector<std::unique_ptr<Component>> m_Components;
        std::vector<GameObject*> m_Childs;
        GameObject* m_Parent = nullptr;
};

class Selection {
public:
    inline static GameObject* selected = nullptr;
};

std::ostream& operator<<(std::ostream& os, const GameObject& obj);
