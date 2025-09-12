#include "Window/Window.h"
#include "core/Input.h"


Hydro::Window window(800, 600, "Prueba");
auto WindowsManager = window.GetWindowObject();
auto InputManager = Hydro::Input();

float pitch = 0.0f;
float yaw = 0.0f;

float lastX = window.GetWidth() / 2;
float lastY = window.GetHeight() / 2;

static void processMouse(double xpos, double ypos) {
    const float sensitivity = 0.2f;

    float deltaX = (xpos - lastX) * sensitivity;
    float deltaY = (ypos - lastY) * sensitivity;

    lastX = xpos;
    lastY = ypos;

    yaw += deltaX;
    pitch += deltaY;

    if (pitch > 89.0f) {
        pitch = 89.0f;
    }
    else if (pitch < -89.0f) {
        pitch = -89.0f;
    }
}

static void processInput(GLFWwindow* window) {
    if (InputManager.IsKeyPressed(Hydro::KeyCode::Escape))
        glfwSetWindowShouldClose(window, true);
}


int main()
{


    while (!window.ShouldClose())
    {
        Hydro::Window::PollEvents();
    }

    return 0;
}