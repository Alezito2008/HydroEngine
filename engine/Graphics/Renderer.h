
#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace Renderer
{
    class Renderer
    {
    public:
        Renderer();
        ~Renderer();

        void SetColor(float a, float b, float c, float d);
        void Clear();
        void Present();
    private:
        float color[4]{};
    };
} // Renderer


