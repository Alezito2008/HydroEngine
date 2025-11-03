#pragma once

#include "Core/WindowManager.h"

namespace GamePanel {
    void Initialize(WindowManager& windowManager, unsigned int width, unsigned int height);
    void Shutdown();

    void Render();
    void Bind();
    void Unbind();
    void Clear(float r = 0.0f, float g = 0.0f, float b = 0.0f);
}
