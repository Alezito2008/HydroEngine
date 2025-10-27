#pragma once

namespace GamePanel {
    extern unsigned int frameBuffer;
    bool InitFramebuffer(unsigned int width, unsigned int height);
    void Render();
}
