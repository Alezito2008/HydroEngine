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
        imgui.StartFrame();
        ShowInterface();
        imgui.EndFrame();
        
        glBindFramebuffer(GL_FRAMEBUFFER, GamePanel::frameBuffer);

        glClearColor(1.0f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        windowManager.PollEventsAndSwapBuffers();
    }

    return 0;
}
