#include "Core/WindowManager.h"
#include "ImGuiLayer.h"
#include "Interface.h"
#include "GamePanel/GamePanel.h"

#include <iostream>

int main() {
    auto windowSettings = WindowSettings("Hydro Engine Editor", 1280, 720);
    auto windowManager  = WindowManager(windowSettings);
    auto* window        = windowManager.GetWindow();
    auto imgui          = ImGuiLayer(window);

    GamePanel::Initialize(windowManager, windowSettings.width, windowSettings.height);

    // ---------------- Main Loop ----------------
    while (!glfwWindowShouldClose(window)) {
        imgui.StartFrame();
        ShowInterface();
        imgui.EndFrame();

        windowManager.PollEventsAndSwapBuffers();
    }

    GamePanel::Shutdown();

    return 0;
}
