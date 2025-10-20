#pragma once
#include "SerializedField.h"
#include <string>
#include <vector>

#define SERIALIZED_FIELD(var) SerializeField(#var, var);

class GameObject;

class Component {
public:
    Component(GameObject* owner);
    virtual ~Component();

    void Destroy();

    inline bool* GetActivePtr() { return &m_IsActive; }
    inline void SetActive(const bool active) { m_IsActive = active; }
    inline bool IsActive() const { return m_IsActive; }

    template <typename T> void SerializeField(const std::string& name, T& ptr) {
        ISerializedField* field = new SerializedField<T>(name, &ptr);
        m_SerializedFields.push_back(field);
    };

    inline const std::vector<ISerializedField*>& GetSerializedFields() const { return m_SerializedFields; }

    virtual const std::string& GetName() const;

    virtual void Start();
    virtual void Update();
    virtual void OnDestroy();

    private:
        static unsigned int m_NextID;
        unsigned int m_ID;
        bool m_IsActive = true;

    protected:
        std::vector<ISerializedField*> m_SerializedFields;
        GameObject* m_Owner;
};
