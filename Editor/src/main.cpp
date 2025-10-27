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

    GamePanel::InitFramebuffer(windowSettings.width, windowSettings.height);

    // ---------------- Main Loop ----------------
    while (!glfwWindowShouldClose(window)) {
        GamePanel::Bind();
        GamePanel::Clear(1.0f, 0.0f, 1.0f);
        // Dibujar aca el juego
        GamePanel::Unbind();

        imgui.StartFrame();
        ShowInterface();
        imgui.EndFrame();

        windowManager.PollEventsAndSwapBuffers();
    }

    return 0;
}
