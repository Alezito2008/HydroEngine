#pragma once
#include <glad/glad.h>

#include "core/Debug.h"
#include "Graphics/VertexArray.h"
#include "Graphics/IndexBuffer.h"
#include "Graphics/Shader.h"
#include "Graphics/VertexArray.h"

namespace Hydro
{
    class IndexBuffer;
    class Shader;
}

class Renderer {
public:
    void Clear() const;
    void Draw(const Hydro::VertexArray& va, const Hydro::Shader& shader, unsigned int count) const;
    void Draw(const Hydro::VertexArray& va, const Hydro::IndexBuffer& ib, const Hydro::Shader& shader) const;
};
