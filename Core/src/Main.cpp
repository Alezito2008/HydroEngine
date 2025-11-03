#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb/stb_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "graphics/Model.h"
#include "graphics/Shader.h"
#include "WindowManager.h"
#include "DemoScene.h"

int main() {
    WindowSettings windowSettings("OpenGL Model Loader", 1500, 1000);
    WindowManager windowManager(windowSettings);
    GLFWwindow* window = windowManager.GetWindow();

    DemoScene scene;
    scene.Initialize(window, windowSettings.width, windowSettings.height);
    windowManager.SetCursorMode(CursorMode::Disabled);

    float lastTime = static_cast<float>(glfwGetTime());

    while (!glfwWindowShouldClose(window)) {
        float currentTime = static_cast<float>(glfwGetTime());
        float deltaTime = currentTime - lastTime;
        lastTime = currentTime;

    const auto& settings = windowManager.GetSettings();
    scene.Resize(static_cast<unsigned int>(settings.width), static_cast<unsigned int>(settings.height));
        scene.Update(deltaTime, true);

        windowManager.PollEventsAndSwapBuffers();
    }

    return 0;
}
