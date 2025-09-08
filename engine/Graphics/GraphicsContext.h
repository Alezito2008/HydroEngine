#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace Hydro
{
    class GraphicsContext
    {
    public:
        explicit GraphicsContext(GLFWwindow* window);
        void Init() const;
        void SwapBuffers() const;
    private:
        GLFWwindow* m_WindowHandle;
    };
}
