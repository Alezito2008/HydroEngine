#include "Scene.h"

unsigned int Scene::s_NextID = 0;

Scene::Scene(const std::string& name) :
    m_ID(s_NextID++),
    m_Name(name.empty() ? ("Scene " + std::to_string(m_ID)) : name) {}

