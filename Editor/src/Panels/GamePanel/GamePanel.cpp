#include "GamePanel.h"
#include "imgui.h"
#include "IconFont/IconsFontAwesome7.h"
#include "glad/glad.h"
#include "Console.h"

unsigned int GamePanel::frameBuffer = 0;
static unsigned int colorTexture = 0;
static unsigned int depthBuffer  = 0;
static unsigned int fbWidth      = 0;
static unsigned int fbHeight     = 0;

bool GamePanel::InitFramebuffer(unsigned int width, unsigned int height)
{
    fbWidth = width;
    fbHeight = height;

    // Crear FrameBuffer
    glGenFramebuffers(1, &frameBuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);

    // Crear textura de color
    glGenTextures(1, &colorTexture);
    glBindTexture(GL_TEXTURE_2D, colorTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, fbWidth, fbHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorTexture, 0);

    // Depth + stencil
    glGenRenderbuffers(1, &depthBuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, depthBuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, fbWidth, fbHeight);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, depthBuffer);

    // Comprobar
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        Console::Error("Error creating GameFrameBuffer");
        return false;
    }

    Console::Debug("GameFrameBuffer created!");
    return true;
}

void GamePanel::Render()
{
    using namespace ImGui;
    Begin(ICON_FA_GAMEPAD " Game###Game");

    ImVec2 avail = GetContentRegionAvail();
    if (avail.x != fbWidth || avail.y != fbHeight) {
        fbWidth = avail.x;
        fbHeight = avail.y;
        InitFramebuffer(fbWidth, fbHeight);
    }
    Image((void*)(intptr_t)colorTexture, avail, ImVec2(0,1), ImVec2(1,0));

    End();
}
