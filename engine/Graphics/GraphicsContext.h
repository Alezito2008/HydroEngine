#pragma once
#include "GLFW/glfw3.h"

namespace GraphicsContext
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
