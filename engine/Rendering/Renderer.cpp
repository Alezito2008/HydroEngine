#include "Renderer.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "Graphics/VertexArray.h"
#include "Graphics/IndexBuffer.h"
#include "Core/Debug.h"
void Renderer::Clear() const
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::Draw(const Hydro::VertexArray& va, const Hydro::IndexBuffer& ib, const Hydro::Shader& shader) const
{
    shader.use();
    va.Bind();
    ib.Bind();
    glDrawElements(GL_TRIANGLES, ib.GetCount(), GL_UNSIGNED_INT, nullptr);
}

void Renderer::Draw(const Hydro::VertexArray& va, const Hydro::Shader& shader, unsigned int count) const
{
    va.Bind();
    shader.use();
    glDrawArrays(GL_TRIANGLES, 0, count);
}