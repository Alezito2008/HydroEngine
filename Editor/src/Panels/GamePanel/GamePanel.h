#pragma once

namespace GamePanel {
    bool InitFramebuffer(unsigned int width, unsigned int height);
    void Render();
    void Bind();
    void Unbind();
    void Clear(float r = 0.0f, float g = 0.0f, float b = 0.0f);
}
