#pragma once

#include "imgui.h"

#include <utility>
#include <functional>
#include <vector>

namespace Fonts {
    extern ImFont* MainFont;
    extern ImFont* MainFontBold;
    extern ImFont* ConsoleFont;
}

struct ThemeEntry {
    const char* name;
    std::function<void()> apply;
};

class Themes {
public:
    static void LoadFonts();
    static void LoadGlobalStyles();
    static void ApplyPreferredTheme();
    
    static void SetDefaultColors();
    static void SetBessDarkColors();
    static void SetCatpuccinMochaColors();
    static void SetModernDarkColors();
    static void SetDarkThemeColors();
    static void SetFluentUIColors();

    static std::vector<ThemeEntry>& GetThemes();

private:
    static inline bool m_SavedDefaultColors = false;
    static void SaveDefaultColors();

    static inline ImGuiStyle m_DefaultStyle;
    static inline ImVec4 m_DefaultColors[ImGuiCol_COUNT];

    static inline std::vector<ImFont*> m_Fonts;

    static inline std::vector<ThemeEntry> m_Themes = {
        { "Default", [](){ SetDefaultColors(); } },
        { "Bess Dark", [](){ SetBessDarkColors(); } },
        { "Catpuccin Mocha", [](){ SetCatpuccinMochaColors(); } },
        { "Modern Dark", [](){ SetModernDarkColors(); } },
        { "Dark", [](){ SetDarkThemeColors(); } },
        { "Fluent UI", [](){ SetFluentUIColors(); } },
    };
};
