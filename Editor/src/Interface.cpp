#include "Interface.h"
#include "imgui_internal.h"
#include "DockLayout.h"
#include "Themes.h"

#include "MenuBar/MenuBar.h"
#include "Console/ConsolePanel.h"
#include "Tree/TreePanel.h"
#include "Inspector/InspectorPanel.h"
#include "GamePanel/GamePanel.h"
#include "Lighting/LightingSettingsPopup.h"

#include "SceneManager.h"
#include "GameObject.h"
#include "GameObject.inl"

static DockLayout layout;

static SceneManager& sceneManager = SceneManager::GetInstance();

static bool firstTime = true;

void ShowInterface() {
    if (firstTime) {
        Themes::LoadGlobalStyles();
        Themes::ApplyPreferredTheme();

        SceneManager& sceneManager = SceneManager::GetInstance();
        sceneManager.GetOrCreateScene("Demo Scene");

        firstTime = false;
    }

    SplitList splitList{
        { "root", ImGuiDir_Right, "###Inspector", "Left", 0.25 },
        { "Left", ImGuiDir_Down, "###Console", "Up", 0.25 },
        { "Up", ImGuiDir_Left, "###Tree", "###Game", 0.3 },
    };
    layout.Setup(splitList);

    MenuBar::Render();
    ConsolePanel::Render();
    TreePanel::Render(sceneManager.GetScenes());
    InspectorPanel::Render();
    GamePanel::Render();
    LightingSettingsPopup::Render();
}
