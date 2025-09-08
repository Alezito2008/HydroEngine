#include "Input.h"
#include <iostream>

namespace Hydro {

    GLFWwindow* Input::s_Window = nullptr;
    std::unordered_map<int, bool> Input::s_KeyStates;
    std::unordered_map<int, bool> Input::s_PrevKeyStates;
    std::unordered_map<int, bool> Input::s_MouseStates;
    std::unordered_map<int, bool> Input::s_PrevMouseStates;

    void Input::Init(GLFWwindow* window) {
        s_Window = window;
    }

    void Input::Update() {
        // Actualizar estados previos
        s_PrevKeyStates = s_KeyStates;
        s_PrevMouseStates = s_MouseStates;
    }

    bool Input::IsKeyPressed(KeyCode key) {
        if (!s_Window) return false;
        int state = glfwGetKey(s_Window, static_cast<int>(key));
        return state == GLFW_PRESS || state == GLFW_REPEAT;
    }

    bool Input::IsKeyJustPressed(KeyCode key) {
        if (!s_Window) return false;
        int keyCode = static_cast<int>(key);
        int state = glfwGetKey(s_Window, keyCode);
        bool currentPressed = (state == GLFW_PRESS || state == GLFW_REPEAT);
        bool prevPressed = s_PrevKeyStates[keyCode];
        return currentPressed && !prevPressed;
    }

    bool Input::IsKeyJustReleased(KeyCode key) {
        if (!s_Window) return false;
        int keyCode = static_cast<int>(key);
        int state = glfwGetKey(s_Window, keyCode);
        bool currentPressed = (state == GLFW_PRESS || state == GLFW_REPEAT);
        bool prevPressed = s_PrevKeyStates[keyCode];
        return !currentPressed && prevPressed;
    }

    bool Input::IsMouseButtonPressed(MouseButton button) {
        if (!s_Window) return false;
        int state = glfwGetMouseButton(s_Window, static_cast<int>(button));
        return state == GLFW_PRESS;
    }

    bool Input::IsMouseButtonJustPressed(MouseButton button) {
        if (!s_Window) return false;
        int buttonCode = static_cast<int>(button);
        int state = glfwGetMouseButton(s_Window, buttonCode);
        bool currentPressed = (state == GLFW_PRESS);
        bool prevPressed = s_PrevMouseStates[buttonCode];
        return currentPressed && !prevPressed;
    }

    bool Input::IsMouseButtonJustReleased(MouseButton button) {
        if (!s_Window) return false;
        int buttonCode = static_cast<int>(button);
        int state = glfwGetMouseButton(s_Window, buttonCode);
        bool currentPressed = (state == GLFW_PRESS);
        bool prevPressed = s_PrevMouseStates[buttonCode];
        return !currentPressed && prevPressed;
    }

    std::pair<float, float> Input::GetMousePosition() {
        if (!s_Window) return {0.0f, 0.0f};
        double x, y;
        glfwGetCursorPos(s_Window, &x, &y);
        return {static_cast<float>(x), static_cast<float>(y)};
    }

    float Input::GetMouseX() {
        auto [x, y] = GetMousePosition();
        return x;
    }

    float Input::GetMouseY() {
        auto [x, y] = GetMousePosition();
        return y;
    }

}
