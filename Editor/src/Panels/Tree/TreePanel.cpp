#include "TreePanel.h"
#include "IconFont/IconsFontAwesome7.h"
#include "Colors.h"

#include <format>

#include "imgui.h"

void RenderGameObject(GameObject* obj) {
    using namespace ImGui;

    const char* name = obj->GetName().c_str();
    const auto& children = obj->GetChilds();
    bool isSelected = Selection::selected == obj;

    char nameIcon[64];
    snprintf(nameIcon, sizeof(nameIcon), "%s %s", ICON_FA_CUBE, name);

    if (children.empty()) {
        if (isSelected) PushStyleColor(ImGuiCol_Header, Color::SelectionBG);

        // Leafs
        TreeNodeEx(nameIcon, ImGuiTreeNodeFlags_Leaf
                                | ImGuiTreeNodeFlags_NoTreePushOnOpen
                                | ImGuiTreeNodeFlags_Framed);

        if (IsItemClicked()) Selection::selected = obj;

        if (isSelected) PopStyleColor();
    } else {
        // Collapsibles
        if (isSelected) PushStyleColor(ImGuiCol_Header, Color::SelectionBG);

        bool open = TreeNodeEx(nameIcon, ImGuiTreeNodeFlags_OpenOnDoubleClick
                                | ImGuiTreeNodeFlags_OpenOnArrow
                                | ImGuiTreeNodeFlags_Framed);

        if (IsItemClicked()) Selection::selected = obj;

        if (isSelected) PopStyleColor();

        if (open) {
            for (const auto& child : children) {
                RenderGameObject(child);
            }

            TreePop();
        }
    }

}

void RenderScene(const Scene* scene) {
    using namespace ImGui;

    const char* sceneName = scene->GetName().c_str();
    char sceneNameIcon[128];
    snprintf(sceneNameIcon, sizeof(sceneNameIcon), "%s %s", ICON_FA_FILM, sceneName);

    Separator();

    PushStyleColor(ImGuiCol_Header, Color::SceneHeaderColor);

    bool open = CollapsingHeader(sceneNameIcon,
                                    ImGuiTreeNodeFlags_DefaultOpen
                                    | ImGuiTreeNodeFlags_OpenOnArrow
                                    | ImGuiTreeNodeFlags_OpenOnDoubleClick);

    PopStyleColor();

    if (open) {
        PushStyleColor(ImGuiCol_Header, IM_COL32(0,0,0,0));
        PushStyleColor(ImGuiCol_HeaderHovered, Color::SelectionBG);
        PushStyleColor(ImGuiCol_HeaderActive, Color::ActiveBG);

        for (const auto& obj : scene->GetGameObjects()) {
            if (obj->GetParent()) continue;
            RenderGameObject(obj.get());
        }

        PopStyleColor(3);
    }

}

void TreePanel::Render(std::vector<std::unique_ptr<Scene>>& scenes) {
    ImGui::Begin( ICON_FA_CODE_BRANCH " Tree ###Tree");

    for (const auto& scene : scenes) {
        RenderScene(scene.get());
    }

    // Deseleccionar si se hace click en cualquier lugar del panel
    if (ImGui::IsWindowHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_Left) && !ImGui::IsAnyItemHovered()) {
        Selection::selected = nullptr;
    }

    ImGui::End();
}
