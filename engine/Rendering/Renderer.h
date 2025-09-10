#pragma once
#include "glm/fwd.hpp"
#include <memory>

namespace Hydro
{
    class Shader;
    class Texture;

    class Renderer
    {
    public:
        static void Init();
        static void Shutdown();
        static void Clear(float r = 0.0f, float g = 0.0f, float b = 0.0f, float a = 1.0f);
        static void BeginScene(const glm::mat4& viewProjection);
        static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color);
        static void DrawQuad(const glm::mat4& transform, const Shader& shader, const Texture& texture);

    private:
        static unsigned int s_QuadVAO;
        static unsigned int s_QuadVBO;
        static unsigned int s_QuadEBO;
        static bool s_Initialized;
        static glm::mat4 s_ViewProjectionMatrix;
        static std::unique_ptr<Shader> s_ColorShader;
    };
}
