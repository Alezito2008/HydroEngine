#include "GraphicsContext.h"

#include <iostream>
#include <stdexcept>

namespace Hydro
{
    GraphicsContext::GraphicsContext(GLFWwindow* window) : m_WindowHandle(window)
    {
        if (!m_WindowHandle)
        {
            throw std::runtime_error("Ventana no valida");
        }
    }
    void GraphicsContext::SwapBuffers() const
    {
        if (m_WindowHandle)
        {
            glfwSwapBuffers(m_WindowHandle);
        }
    }

    void GraphicsContext::Init() const
    {
        glfwMakeContextCurrent(m_WindowHandle);

        if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
            throw std::runtime_error("Error al iniciar GLAD!");

        std::cout << "GLAD iniciado" << std::endl;

        glEnable(GL_DEPTH_TEST);
    }

}
