#include "MenuBar.h"
#include "imgui.h"
#include "Themes.h"
#include "IconFont/IconsFontAwesome7.h"
#include "Preferences.h"
#include "Lighting/LightingSettingsPopup.h"
#include "GamePanel/GamePanel.h"

static Preferences& preferences = PreferenceManager::GetPreferences();

static void Preferences_Theme() {
    using namespace ImGui;

    static std::vector<ThemeEntry> themes = Themes::GetThemes();
    ImVec2 fullWidth(GetContentRegionAvail().x, 0);

    for (const auto& theme : themes) {
        if (Button(theme.name, fullWidth)) {
            theme.apply();
            preferences.theme = theme.name;
            PreferenceManager::SavePreferences();
        }
    }
}

void MenuBar::Render() {
    using namespace ImGui;

    if (BeginMainMenuBar()) {
        if (BeginMenu( ICON_FA_FILE" File")) {
            EndMenu();
        }

        if (BeginMenu( ICON_FA_WRENCH " Preferences")) {
            if (BeginMenu( ICON_FA_BRUSH " Theme")) {
                Preferences_Theme();
                EndMenu();
            }
            EndMenu();
        }

        if (BeginMenu( ICON_FA_LIGHTBULB " Scene")) {
            if (MenuItem("Lighting Settings...")) {
                LightingSettingsPopup::Open();
            }
            EndMenu();
        }

        if (BeginMenu( ICON_FA_GAMEPAD " Game")) {
            bool hasScene = GamePanel::GetScene() != nullptr;
            bool paused = GamePanel::IsPaused();

            if (MenuItem(paused ? ICON_FA_PLAY " Resume" : ICON_FA_PAUSE " Pause", nullptr, false, hasScene)) {
                GamePanel::TogglePause();
            }

            if (MenuItem( ICON_FA_ROTATE_RIGHT " Restart Run", nullptr, false, hasScene)) {
                GamePanel::Restart();
            }

            if (MenuItem( ICON_FA_ARROWS_ROTATE " Reload Scene", nullptr, false, hasScene)) {
                GamePanel::Reload();
            }

            EndMenu();
        }

        EndMainMenuBar();
    }
}
