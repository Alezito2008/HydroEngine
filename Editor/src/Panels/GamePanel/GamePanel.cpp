#include "GamePanel.h"

#include "imgui.h"
#include "IconFont/IconsFontAwesome7.h"
#include "glad/glad.h"

#include <algorithm>

#include "Console.h"
#include "Core/DemoScene.h"

static unsigned int frameBuffer  = 0;
static unsigned int colorTexture = 0;
static unsigned int depthBuffer  = 0;
static unsigned int fbWidth      = 0;
static unsigned int fbHeight     = 0;

static DemoScene demoScene;
static bool sceneInitialized       = false;
static bool framebufferReady       = false;
static WindowManager* windowOwner  = nullptr;
static GLFWwindow* glfwWindow      = nullptr;
static double lastFrameTime        = 0.0;
static bool capturingInput         = false;

namespace {
    void DestroyFramebuffer()
    {
        if (depthBuffer != 0) {
            glDeleteRenderbuffers(1, &depthBuffer);
            depthBuffer = 0;
        }

        if (colorTexture != 0) {
            glDeleteTextures(1, &colorTexture);
            colorTexture = 0;
        }

        if (frameBuffer != 0) {
            glDeleteFramebuffers(1, &frameBuffer);
            frameBuffer = 0;
        }
    }

    bool InitFramebufferInternal(unsigned int width, unsigned int height)
    {
        if (width == 0 || height == 0) {
            return false;
        }

        DestroyFramebuffer();

        fbWidth  = width;
        fbHeight = height;

        glGenFramebuffers(1, &frameBuffer);
        glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);

        glGenTextures(1, &colorTexture);
        glBindTexture(GL_TEXTURE_2D, colorTexture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, fbWidth, fbHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorTexture, 0);

        glGenRenderbuffers(1, &depthBuffer);
        glBindRenderbuffer(GL_RENDERBUFFER, depthBuffer);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, fbWidth, fbHeight);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, depthBuffer);

        bool complete = glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE;

        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        return complete;
    }

    void RestoreWindowViewport()
    {
        if (windowOwner != nullptr) {
            const auto& settings = windowOwner->GetSettings();
            glViewport(0, 0, settings.width, settings.height);
            return;
        }

        if (glfwWindow != nullptr) {
            int width = 0;
            int height = 0;
            glfwGetFramebufferSize(glfwWindow, &width, &height);
            glViewport(0, 0, width, height);
        }
    }
}

void GamePanel::Initialize(WindowManager& windowManager, unsigned int width, unsigned int height)
{
    windowOwner = &windowManager;
    glfwWindow  = windowManager.GetWindow();

    framebufferReady = InitFramebufferInternal(width, height);
    if (!framebufferReady) {
        Console::Error("[GamePanel] No se pudo crear el framebuffer del juego");
        return;
    }

    demoScene.Initialize(glfwWindow, fbWidth, fbHeight);
    sceneInitialized = true;
    lastFrameTime = glfwGetTime();
}

void GamePanel::Shutdown()
{
    DestroyFramebuffer();
    framebufferReady = false;
    sceneInitialized  = false;
    windowOwner       = nullptr;
    glfwWindow        = nullptr;
    capturingInput    = false;
}

void GamePanel::Render()
{
    using namespace ImGui;

    Begin(ICON_FA_GAMEPAD " Game###Game");

    ImVec2 avail = GetContentRegionAvail();
    unsigned int targetWidth  = static_cast<unsigned int>(std::max(1.0f, avail.x));
    unsigned int targetHeight = static_cast<unsigned int>(std::max(1.0f, avail.y));

    if ((targetWidth != fbWidth || targetHeight != fbHeight) && targetWidth > 0 && targetHeight > 0) {
        framebufferReady = InitFramebufferInternal(targetWidth, targetHeight);
        if (framebufferReady && sceneInitialized) {
            demoScene.Resize(fbWidth, fbHeight);
        }
    }

    bool windowHovered = IsWindowHovered(ImGuiHoveredFlags_AllowWhenBlockedByActiveItem | ImGuiHoveredFlags_RootAndChildWindows);
    bool rightMouseDown = ImGui::IsMouseDown(ImGuiMouseButton_Right);

    if (sceneInitialized) {
        if (!capturingInput && windowHovered && rightMouseDown) {
            capturingInput = true;
            if (windowOwner) {
                windowOwner->SetCursorMode(CursorMode::Disabled);
            }
            lastFrameTime = glfwGetTime();
        } else if (capturingInput && !rightMouseDown) {
            capturingInput = false;
            if (windowOwner) {
                windowOwner->SetCursorMode(CursorMode::Normal);
            }
        }
    }

    bool allowInput = capturingInput && sceneInitialized;

    if (sceneInitialized && framebufferReady) {
        double currentTime = glfwGetTime();
        float deltaTime = static_cast<float>(currentTime - lastFrameTime);
        lastFrameTime = currentTime;

        Bind();
        demoScene.Resize(fbWidth, fbHeight);
        demoScene.Update(deltaTime, allowInput);
        Unbind();

        RestoreWindowViewport();
    }

    Image((void*)(intptr_t)colorTexture, avail, ImVec2(0, 1), ImVec2(1, 0));

    End();
}

void GamePanel::Bind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
}

void GamePanel::Unbind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void GamePanel::Clear(float r, float g, float b)
{
    glClearColor(r, g, b, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
