#include "Core/WindowManager.h"
#include "ImGuiLayer.h"
#include "Interface.h"

#include <iostream>

int main() {
    auto windowSettings = WindowSettings("Hydro Engine Editor", 1280, 720);
    auto windowManager  = WindowManager(windowSettings);
    auto* window        = windowManager.GetWindow();
    auto imgui          = ImGuiLayer(window);

    // ---------------- Main Loop ----------------
    while (!glfwWindowShouldClose(window)) {        
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        imgui.StartFrame();
        ShowInterface();
        imgui.EndFrame();

        windowManager.PollEventsAndSwapBuffers();
    }

    return 0;
}
