#pragma once
#include <string>
#include <typeinfo>

struct ISerializedField {
    virtual const char* GetName() const = 0;
    virtual void* GetPtr() = 0;
    virtual const std::type_info& GetType() const = 0;
};

template <typename T>
struct SerializedField : ISerializedField {
    std::string name;
    T* ptr;

    SerializedField(const std::string& name, T* ptr) : name(name), ptr(ptr) {}

    inline const char* GetName() const override { return name.c_str(); }
    inline void* GetPtr() override { return ptr; }
    inline const std::type_info& GetType() const override { return typeid(T); }

};
