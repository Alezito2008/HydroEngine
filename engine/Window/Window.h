#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace Hydro
{
    class Window
    {
    public:
        Window(int width, int height, const char* title);
        ~Window();

        static void PollEvents();
        bool GetShouldClose() const;
        void SetShouldClose(bool should) const;
        int GetWidth() const;
        int GetHeight() const;
        GLFWwindow* GetWindowObject() const;

    private:
        int m_Width;
        int m_Height;
        const char* m_Title;
        GLFWwindow* m_Window;

        void Init();
        void Shutdown();

        static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
    };
}
