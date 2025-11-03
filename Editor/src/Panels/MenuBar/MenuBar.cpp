#include "MenuBar.h"
#include "imgui.h"
#include "Themes.h"
#include "IconFont/IconsFontAwesome7.h"
#include "Preferences.h"
#include "Lighting/LightingSettingsPopup.h"

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

        EndMainMenuBar();
    }
}
