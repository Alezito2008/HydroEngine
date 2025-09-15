
#include "Window.h"

#include <stdexcept>

namespace Hydro
{

    Window::Window(int width, int height, const char* title)
        : m_Width(width), m_Height(height), m_Title(title), m_Window(nullptr)
    {
        Init();
    }

    Window::~Window()
    {
        Shutdown();
    }

    void Window::framebuffer_size_callback(GLFWwindow* window, int width, int height)
    {
        glViewport(0, 0, width, height);
    }



    void Window::Init()
    {
        if (!glfwInit())
        {
            // Initialization failed
            throw std::runtime_error("Error al iniciar GLFW");
        }

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        m_Window = glfwCreateWindow(m_Width, m_Height, m_Title, nullptr, nullptr);
        if (!m_Window)
        {
            glfwTerminate();
            throw std::runtime_error("Error al crear la ventana GLFW");
        }

        glfwSetFramebufferSizeCallback(m_Window,  framebuffer_size_callback);

    }

    void Window::Shutdown()
    {
        if (m_Window)
        {
            glfwDestroyWindow(m_Window);
            m_Window = nullptr;
        }
        glfwTerminate();
    }

    void Window::PollEvents()
    {
        glfwPollEvents();
    }


    bool Window::GetShouldClose() const
    {
        return m_Window ? glfwWindowShouldClose(m_Window) : true;
    }
    void Window::SetShouldClose(bool should) const
    {
        glfwSetWindowShouldClose(m_Window, should);
    }


    int Window::GetWidth() const
    {
        return m_Width;
    }

    int Window::GetHeight() const
    {
        return m_Height;
    }

    GLFWwindow* Window::GetWindowObject() const
    {
        return m_Window;
    }
}