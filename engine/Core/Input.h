#pragma once
#include <GLFW/glfw3.h>
#include <unordered_map>

namespace Hydro {

    enum class KeyCode {
        // Letras
        A = GLFW_KEY_A, B = GLFW_KEY_B, C = GLFW_KEY_C, D = GLFW_KEY_D,
        E = GLFW_KEY_E, F = GLFW_KEY_F, G = GLFW_KEY_G, H = GLFW_KEY_H,
        I = GLFW_KEY_I, J = GLFW_KEY_J, K = GLFW_KEY_K, L = GLFW_KEY_L,
        M = GLFW_KEY_M, N = GLFW_KEY_N, O = GLFW_KEY_O, P = GLFW_KEY_P,
        Q = GLFW_KEY_Q, R = GLFW_KEY_R, S = GLFW_KEY_S, T = GLFW_KEY_T,
        U = GLFW_KEY_U, V = GLFW_KEY_V, W = GLFW_KEY_W, X = GLFW_KEY_X,
        Y = GLFW_KEY_Y, Z = GLFW_KEY_Z,

        // Números
        Key0 = GLFW_KEY_0, Key1 = GLFW_KEY_1, Key2 = GLFW_KEY_2,
        Key3 = GLFW_KEY_3, Key4 = GLFW_KEY_4, Key5 = GLFW_KEY_5,
        Key6 = GLFW_KEY_6, Key7 = GLFW_KEY_7, Key8 = GLFW_KEY_8,
        Key9 = GLFW_KEY_9,

        // Teclas especiales
        Space = GLFW_KEY_SPACE,
        Enter = GLFW_KEY_ENTER,
        Tab = GLFW_KEY_TAB,
        Backspace = GLFW_KEY_BACKSPACE,
        Delete = GLFW_KEY_DELETE,
        Escape = GLFW_KEY_ESCAPE,

        // Flechas
        Left = GLFW_KEY_LEFT,
        Right = GLFW_KEY_RIGHT,
        Up = GLFW_KEY_UP,
        Down = GLFW_KEY_DOWN,

        // Modificadores
        LeftShift = GLFW_KEY_LEFT_SHIFT,
        RightShift = GLFW_KEY_RIGHT_SHIFT,
        LeftCtrl = GLFW_KEY_LEFT_CONTROL,
        RightCtrl = GLFW_KEY_RIGHT_CONTROL,
        LeftAlt = GLFW_KEY_LEFT_ALT,
        RightAlt = GLFW_KEY_RIGHT_ALT
    };

    enum class MouseButton {
        Left = GLFW_MOUSE_BUTTON_LEFT,
        Right = GLFW_MOUSE_BUTTON_RIGHT,
        Middle = GLFW_MOUSE_BUTTON_MIDDLE
    };

    class Input {
    public:
        // Teclado
        static bool IsKeyPressed(KeyCode key);
        static bool IsKeyJustPressed(KeyCode key);
        static bool IsKeyJustReleased(KeyCode key);

        // Mouse
        static bool IsMouseButtonPressed(MouseButton button);
        static bool IsMouseButtonJustPressed(MouseButton button);
        static bool IsMouseButtonJustReleased(MouseButton button);
        static std::pair<float, float> GetMousePosition();
        static float GetMouseX();
        static float GetMouseY();

        static void Init(GLFWwindow* window);
        static void Update();

    private:
        static GLFWwindow* s_Window;
        static std::unordered_map<int, bool> s_KeyStates;
        static std::unordered_map<int, bool> s_PrevKeyStates;
        static std::unordered_map<int, bool> s_MouseStates;
        static std::unordered_map<int, bool> s_PrevMouseStates;
    };

}
