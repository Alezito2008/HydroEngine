#include "ConsolePanel.h"
#include "IconFont/IconsFontAwesome7.h"
#include "Preferences.h"
#include "Themes.h"

#include "imgui.h"


static char inputBuffer[512];
static ImGuiTextBuffer logBuffer;
static ImGuiTextFilter filter;

static Preferences& preferences = PreferenceManager::GetPreferences();

static bool autoScroll = preferences.autoScroll;
static bool scrollToBottom = false;

void ConsolePanel::AddLog(const std::string& msg, LogType type) {
    logBuffer.appendf("%s\n", msg.c_str());
    scrollToBottom = autoScroll;
}

void ConsolePanel::Render() {
    using namespace ImGui;
    Begin(ICON_FA_TERMINAL " Console ###Console");

    if (BeginTable("ConsoleTable", 2, ImGuiTableFlags_SizingStretchProp)) {
        TableNextColumn();

        if (BeginPopup("Console Options")) {
            if (Checkbox( "AutoScroll", &autoScroll)) {
                preferences.autoScroll = autoScroll;
                PreferenceManager::SavePreferences();
            };
            EndPopup();
        }

        SetNextItemWidth(GetContentRegionAvail().x);

        PushFont(Fonts::ConsoleFont, 14.0f);

        filter.Draw("##filter");

        PopFont();
        
        TableNextColumn();
        if (Button( ICON_FA_BROOM " Clear")) {
            logBuffer.clear();
        };
        if (IsItemHovered()) {
            SetTooltip("Clear all logs");
        };
        SameLine();
        if (Button( ICON_FA_COPY " Copy")) {
            LogToClipboard();
            TextUnformatted(logBuffer.begin(),logBuffer.end());
            LogFinish();
        };
        if (IsItemHovered()) {
            SetTooltip("Copy all logs");
        };
        SameLine();
        if (Button(ICON_FA_GEAR " Options")) {
            OpenPopup("Console Options");
        };
        if (IsItemHovered()) {
            SetTooltip("Display options");
        };
        EndTable();
    }

    BeginChild("LogRegion", ImVec2(0,0), true, ImGuiWindowFlags_HorizontalScrollbar | ImGuiWindowFlags_AlwaysVerticalScrollbar);
    const char* text = logBuffer.begin();
    const char* line_start = text;

    int line_index = 0;
    float width = GetContentRegionAvail().x;

    PushFont(Fonts::ConsoleFont, 14.0f);

    for (const char* p = text; *p; p++) {
        if (*p == '\n') {
            std::string line(line_start, p - line_start);
            if (filter.PassFilter(line.c_str())) {
                ImVec2 pos = GetCursorScreenPos();
                ImVec2 text_size = CalcTextSize(line.c_str(), nullptr, false, width);

                if (line_index % 2 == 1) {
                    GetWindowDrawList()->AddRectFilled(
                        pos,
                        ImVec2(width + pos.x, text_size.y + pos.y),
                        IM_COL32(100, 100, 100, 50)
                    );
                }

                PushTextWrapPos(width);
                TextUnformatted(line.c_str());
                PopTextWrapPos();


                line_index++;
            }
            line_start = p + 1;
        }
    }

    PopFont();

    if (scrollToBottom) {
        SetScrollHereY(1.0f);
        scrollToBottom = false;
    }

    EndChild();

    End();
}
