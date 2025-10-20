#include "DockLayout.h"

#include "imgui.h"
#include "imgui_internal.h"

void DockLayout::Setup(const SplitList& splits) {
    ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");

    float menuBarHeight = ImGui::GetFrameHeight();

    ImGui::SetNextWindowPos(ImVec2(viewport->Pos.x, viewport->Pos.y + menuBarHeight));
    ImGui::SetNextWindowSize(ImVec2(viewport->Size.x, viewport->Size.y - menuBarHeight));
    ImGui::SetNextWindowViewport(viewport->ID);

    ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse |
                                    ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
                                    ImGuiWindowFlags_NoBringToFrontOnFocus |
                                    ImGuiWindowFlags_NoNavFocus | ImGuiWindowFlags_NoDocking;

    ImGui::Begin("DockSpaceWindow", nullptr, window_flags);
    ImGui::DockSpace(dockspace_id, ImVec2(0,0), ImGuiDockNodeFlags_None);

    if (m_FirstTime) {
        m_FirstTime = false;

        // Parent node
        ImGui::DockBuilderRemoveNode(dockspace_id);
        ImGui::DockBuilderAddNode(dockspace_id, ImGuiDockNodeFlags_DockSpace);
        ImGui::DockBuilderSetNodeSize(dockspace_id, viewport->Size);

        RegisterNode("root", dockspace_id);

        for (const auto split : splits) {
            Split(split.parent, split.direction, split.first, split.second, split.ratio);
        }

        ImGui::DockBuilderFinish(dockspace_id);
    }

    ImGui::End();
}

void DockLayout::Split(const char* parentNode, ImGuiDir direction, const char* first, const char* second, float ratio) {
    ImGuiID first_id = 0, second_id = 0;

    ImGui::DockBuilderSplitNode(GetNodeID(parentNode), direction, ratio, &first_id, &second_id);

    if (first) {
        ImGui::DockBuilderDockWindow(first, first_id);
        RegisterNode(first, first_id);
    }

    if (second) {
        ImGui::DockBuilderDockWindow(second, second_id);
        RegisterNode(second, second_id);
    }
}

void DockLayout::RegisterNode(const std::string& nodeName, ImGuiID id) {
    m_NodeMap[nodeName] = id;
};

ImGuiID DockLayout::GetNodeID(const std::string& nodeName) {
    if (auto it = m_NodeMap.find(nodeName); it != m_NodeMap.end()) {
        return it->second;
    }
    return 0;
};
