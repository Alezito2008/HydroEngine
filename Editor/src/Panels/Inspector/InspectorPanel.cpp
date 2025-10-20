#include "Inspector/InspectorPanel.h"
#include "IconFont/IconsFontAwesome7.h"
#include "imgui.h"
#include "Colors.h"
#include "Themes.h"
#include "GameObject.h"
#include "GameObject.inl"
#include "SerializedField.h"
#include "ComponentSelector/ComponentSelectorPopup.h"

const std::string ToTag(const std::string& name) {
    return "###" + name + "_field";
} 

static void DrawSerializedField(ISerializedField* field, bool isEnabled) {
    using namespace ImGui;
    const char* name = field->GetName();

    if (!isEnabled) BeginDisabled();

    if (field->GetType() == typeid(int)) {
        int* value = static_cast<int*>(field->GetPtr());
        Text("%s:", name);
        SameLine();
        SetNextItemWidth(100);
        InputInt(ToTag(name).c_str(), value);
    }

    if (!isEnabled) EndDisabled();
}

static void RenderObjectInfo(GameObject* obj, bool isActive) {
    using namespace ImGui;

    const std::string name = " " + obj->GetName();

    PushFont(Fonts::MainFontBold, 16.0f);
    // Titulo
    Checkbox(name.c_str(), obj->GetActivePtr());
    PopFont();

    if (!isActive) ImGui::BeginDisabled();

    SeparatorText("Properties");

    Dummy(ImVec2(0, 5.0f));
    
    PushFont(Fonts::ConsoleFont, 14.0f);

    GameObject* parent = obj->GetParent();

    Text("ID: %d", obj->GetID());
    Text("Childs: %d", obj->GetChilds().size());
    Text("Components: %d", obj->GetComponents().size());
    Text("Parent: %s", parent ? parent->GetName().c_str() : "null");

    PopFont();

    Dummy(ImVec2(0, 5.0f));
}

static void RenderComponent(Component* comp) {
    using namespace ImGui;

    const std::string name = " " + comp->GetName();
    const bool active = comp->IsActive();

    PushID(comp);

    Dummy(ImVec2(0, 2.5f));
    PushFont(Fonts::MainFontBold, 16.0f);
    // Titulo
    Checkbox(name.c_str(), comp->GetActivePtr());
    PopFont();

    PushFont(Fonts::ConsoleFont, 16.0f);
    // Fields
    for (ISerializedField* field : comp->GetSerializedFields()) {
        Dummy(ImVec2(0, 2.5f));
        DrawSerializedField(field, active);
    }
    PopID();
    PopFont();
    Dummy(ImVec2(0, 2.5f));
    Separator();
}

static void ComponentSection(GameObject* obj) {
    using namespace ImGui;

    SeparatorText("Components");

    for (auto& comp : obj->GetComponents()) {
        RenderComponent(comp);
    }

    Dummy(ImVec2(0, 2.5f));
    if (Button(" + Add Component", ImVec2(-FLT_MIN, 0))) ComponentSelectorPopup::Open();
    ComponentSelectorPopup::Render();
}

void InspectorPanel::Render() {
    ImGui::Begin( ICON_FA_CIRCLE_INFO " Inspector ###Inspector");

    ImGui::PushTextWrapPos(0.0f);

    GameObject* obj = Selection::selected;

    if (obj) {
        const bool isActive = obj->IsActive();
        
        RenderObjectInfo(obj, isActive);
        
        ComponentSection(obj);

        if (!isActive) ImGui::EndDisabled();
    } else {
        // Cuando no hay nada seleccionado
        constexpr const char* text = "No Game Object selected";

        const ImVec2 spaceSize = ImGui::GetWindowSize();
        const ImVec2 textSize = ImGui::CalcTextSize(text, nullptr, false, 0.0f);
        float cursorX = (spaceSize.x - textSize.x) * 0.5f;
        const float cursorY = (spaceSize.y - textSize.y) * 0.5f;

        if (cursorX <= 15.0f) cursorX = 15.0f;

        ImGui::SetCursorPos(ImVec2(cursorX, cursorY));
        
        ImGui::PushStyleColor(ImGuiCol_Text, Color::SecondaryText);
        ImGui::TextUnformatted(text);
        ImGui::PopStyleColor();
    }

    ImGui::PopTextWrapPos();

    ImGui::End();
}
