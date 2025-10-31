#include "ComponentSelector/ComponentSelectorPopup.h"
#include "IconFont/IconsFontAwesome7.h"
#include "imgui.h"
#include "ComponentRegistry.h"
#include "Themes.h"
#include "GameObject.h"
#include "Component.h"

static bool isOpen = false;

void ComponentSelectorPopup::Open() {
    isOpen = true;
    ImGui::OpenPopup("###component-selector");
}

void ComponentSelectorPopup::Render() {
    using namespace ImGui;

    if (!isOpen) return;

    if (BeginPopup("Select Component ###component-selector")) {
        PushFont(Fonts::MainFontBold, 16.0f);
        AlignTextToFramePadding();
        Text("Select Component");
        PopFont();
        SameLine();
        // Boton recargar
        if (Button(ICON_FA_ROTATE_RIGHT)) {
            ComponentRegistry::LoadLibrary("build/Components/libComponents.so");
        };
        Separator();
        
        Spacing();
        
        const auto& components = ComponentRegistry::GetComponents();

        static float lineHeight = GetTextLineHeightWithSpacing();
        static float maxHeight = 300.0f;

        float childHeight = lineHeight * components.size();

        childHeight = std::min(childHeight, maxHeight) + 10.0f;

        BeginChild("components-view", ImVec2(0, childHeight));

        for (const auto& [name, create] : components) {
            if (Button(name.c_str(), ImVec2(-FLT_MIN, 0))) {
                std::unique_ptr<Component> comp = create(Selection::selected);
                Selection::selected->AddComponent(std::move(comp));
                CloseCurrentPopup();
                isOpen = false;
            };
        }

        EndChild();

        EndPopup();
    };
    
}
