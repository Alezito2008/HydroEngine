#include "Lighting/LightingSettingsPopup.h"

#include "IconFont/IconsFontAwesome7.h"
#include "imgui.h"

#include <glm/gtc/type_ptr.hpp>

#include "GamePanel/GamePanel.h"
#include "Core/DemoScene.h"

namespace {
    constexpr ImGuiWindowFlags kPopupFlags = ImGuiWindowFlags_AlwaysAutoResize;
    constexpr const char* kPopupName = ICON_FA_LIGHTBULB " Lighting Settings###lighting-settings";
    bool s_requestOpen = false;

    void ResetToDefaults(DemoScene::LightingSettings& settings)
    {
        settings.color = {1.0f, 1.0f, 1.0f};
        settings.intensity = 1.0f;
        settings.ambientStrength = 0.2f;
        settings.shininess = 32.0f;
    }
}

void LightingSettingsPopup::Open()
{
    s_requestOpen = true;
}

void LightingSettingsPopup::Render()
{
    if (s_requestOpen) {
        ImGui::OpenPopup(kPopupName);
        s_requestOpen = false;
    }

    bool open = true;
    if (ImGui::BeginPopupModal(kPopupName, &open, kPopupFlags)) {
        DemoScene* scene = GamePanel::GetScene();
        if (scene == nullptr) {
            ImGui::TextUnformatted("La escena no está inicializada.");
        } else {
            auto& settings = scene->GetLightingSettings();

            glm::vec3 color = settings.color;
            if (ImGui::ColorEdit3("Color de luz", glm::value_ptr(color), ImGuiColorEditFlags_DisplayRGB | ImGuiColorEditFlags_Float)) {
                settings.color = color;
            }

            float intensity = settings.intensity;
            if (ImGui::SliderFloat("Intensidad", &intensity, 0.0f, 10.0f, "%.2f")) {
                settings.intensity = intensity;
            }

            float ambient = settings.ambientStrength;
            if (ImGui::SliderFloat("Ambient", &ambient, 0.0f, 1.0f, "%.2f")) {
                settings.ambientStrength = ambient;
            }

            float shininess = settings.shininess;
            if (ImGui::SliderFloat("Shininess", &shininess, 1.0f, 256.0f, "%.0f")) {
                settings.shininess = shininess;
            }

            ImGui::Separator();

            if (ImGui::Button("Restablecer")) {
                ResetToDefaults(settings);
            }
        }

        ImGui::Separator();

        if (ImGui::Button("Cerrar", ImVec2(120.0f, 0.0f))) {
            ImGui::CloseCurrentPopup();
            open = false;
        }

        ImGui::EndPopup();
    }
}
