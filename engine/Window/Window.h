#pragma once


#include <GLFW/glfw3.h>

namespace Window
{
    class Window
    {
    public:
        Window(int width, int height, const char* title);
        ~Window();

        void PollEvents();
        void SwapBuffers();
        bool ShouldClose() const;

        int GetWidth() const;
        int GetHeight() const;

        void* GetWindowObject() const;

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

