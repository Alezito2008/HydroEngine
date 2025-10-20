#pragma once

#include "imgui.h"

#include <unordered_map>
#include <vector>
#include <string>

struct NodeSplit {
    const char* parent;
    ImGuiDir direction;
    const char* first;
    const char* second;
    float ratio;
};

using SplitList = std::vector<NodeSplit>;

class DockLayout {
public:
    DockLayout() = default;
    ~DockLayout() = default;

    void RegisterNode(const std::string& nodeName, ImGuiID id);
    ImGuiID GetNodeID(const std::string& nodeName);

    void Setup(const SplitList& splits);

    void Split(const char* parentNode, ImGuiDir direction, const char* first, const char* second, float ratio);
private:
    bool m_FirstTime = true;
    const char* m_DockspaceName;
    std::unordered_map<std::string, ImGuiID> m_NodeMap;
};
