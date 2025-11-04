#include "GamePanel.h"

#include "imgui.h"
#include "IconFont/IconsFontAwesome7.h"
#include "glad/glad.h"

#include <algorithm>
#include <cstdio>
#include <string>
#include <vector>

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
static bool paused                 = false;

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
    capturingInput = false;
    paused = false;
}

void GamePanel::Shutdown()
{
    DestroyFramebuffer();
    framebufferReady = false;
    sceneInitialized  = false;
    windowOwner       = nullptr;
    glfwWindow        = nullptr;
    capturingInput    = false;
    paused            = false;
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
        if (paused) {
            if (capturingInput) {
                capturingInput = false;
                if (windowOwner) {
                    windowOwner->SetCursorMode(CursorMode::Normal);
                }
            }
        } else {
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
    }

    bool allowInput = capturingInput && sceneInitialized && !paused;

    if (sceneInitialized && framebufferReady) {
        double currentTime = glfwGetTime();
        float deltaTime = static_cast<float>(currentTime - lastFrameTime);
        lastFrameTime = currentTime;

        if (paused) {
            deltaTime = 0.0f;
            allowInput = false;
        }

        Bind();
        demoScene.Resize(fbWidth, fbHeight);
        demoScene.Update(deltaTime, allowInput);
        Unbind();

        RestoreWindowViewport();
    }

    Image((void*)(intptr_t)colorTexture, avail, ImVec2(0, 1), ImVec2(1, 0));

    if (sceneInitialized && framebufferReady) {
        const DemoScene::GameplayStats& stats = demoScene.GetStats();
        const std::string& waveMessage = demoScene.GetWaveMessage();

        ImDrawList* drawList = ImGui::GetWindowDrawList();
        ImVec2 imageMin = ImGui::GetItemRectMin();
        ImVec2 imageMax = ImGui::GetItemRectMax();

        if (drawList != nullptr) {
            std::vector<std::string> hudLines;
            hudLines.reserve(9);

            char buffer[64];
            std::snprintf(buffer, sizeof(buffer), "Score: %d", stats.score);
            hudLines.emplace_back(buffer);
            std::snprintf(buffer, sizeof(buffer), "Wave: %d", stats.wave);
            hudLines.emplace_back(buffer);
            std::snprintf(buffer, sizeof(buffer), "Collected: %d / %d", stats.collected, stats.total);
            hudLines.emplace_back(buffer);

            int totalSeconds = std::max(0, static_cast<int>(stats.elapsed));
            int minutes = totalSeconds / 60;
            int seconds = totalSeconds % 60;
            std::snprintf(buffer, sizeof(buffer), "Time: %02d:%02d", minutes, seconds);
            hudLines.emplace_back(buffer);

            hudLines.emplace_back("");
            hudLines.emplace_back("Right click to control camera");
            hudLines.emplace_back("WASD to move, Shift to sprint");
            hudLines.emplace_back("Collect the mini backpacks nearby");
            hudLines.emplace_back("Press R to reset the run");

            ImVec2 padding(14.0f, 12.0f);
            ImVec2 cursorPadding(12.0f, 10.0f);

            float hudWidth = 0.0f;
            float hudHeight = cursorPadding.y * 2.0f;

            for (const auto& line : hudLines) {
                ImVec2 textSize = ImGui::CalcTextSize(line.c_str());
                hudWidth = std::max(hudWidth, textSize.x);
                hudHeight += textSize.y;
                hudHeight += 2.0f;
            }

            hudWidth += cursorPadding.x * 2.0f;

            ImVec2 hudPos = ImVec2(imageMin.x + padding.x, imageMin.y + padding.y);
            ImVec2 hudRectMax = ImVec2(hudPos.x + hudWidth, hudPos.y + hudHeight);

            drawList->AddRectFilled(hudPos, hudRectMax, IM_COL32(15, 18, 28, 200), 8.0f);
            drawList->AddRect(hudPos, hudRectMax, IM_COL32(80, 110, 255, 180), 8.0f, ImDrawFlags_None, 1.5f);

            float textY = hudPos.y + cursorPadding.y;
            for (const auto& line : hudLines) {
                ImVec2 textPos(hudPos.x + cursorPadding.x, textY);
                drawList->AddText(textPos, IM_COL32(235, 239, 255, 255), line.c_str());
                ImVec2 textSize = ImGui::CalcTextSize(line.c_str());
                textY += textSize.y + 2.0f;
            }

            if (!waveMessage.empty()) {
                ImVec2 messagePadding(16.0f, 10.0f);
                ImVec2 messageSize = ImGui::CalcTextSize(waveMessage.c_str());
                ImVec2 messagePos = ImVec2((imageMin.x + imageMax.x) * 0.5f, imageMin.y + 35.0f);
                ImVec2 messageMin = ImVec2(messagePos.x - messageSize.x * 0.5f - messagePadding.x, messagePos.y - messagePadding.y);
                ImVec2 messageMax = ImVec2(messagePos.x + messageSize.x * 0.5f + messagePadding.x, messagePos.y + messageSize.y + messagePadding.y);

                drawList->AddRectFilled(messageMin, messageMax, IM_COL32(30, 32, 52, 220), 10.0f);
                drawList->AddRect(messageMin, messageMax, IM_COL32(120, 150, 255, 200), 10.0f, ImDrawFlags_None, 1.5f);
                ImVec2 textPos = ImVec2(messagePos.x - messageSize.x * 0.5f, messagePos.y);
                drawList->AddText(textPos, IM_COL32(240, 243, 255, 255), waveMessage.c_str());
            }

            if (paused) {
                const char* pausedLabel = "Paused";
                ImVec2 labelSize = ImGui::CalcTextSize(pausedLabel);
                ImVec2 labelPadding(28.0f, 14.0f);
                ImVec2 center = ImVec2((imageMin.x + imageMax.x) * 0.5f, (imageMin.y + imageMax.y) * 0.5f);
                ImVec2 pausedMin = ImVec2(center.x - labelSize.x * 0.5f - labelPadding.x, center.y - labelSize.y * 0.5f - labelPadding.y);
                ImVec2 pausedMax = ImVec2(center.x + labelSize.x * 0.5f + labelPadding.x, center.y + labelSize.y * 0.5f + labelPadding.y);

                drawList->AddRectFilled(pausedMin, pausedMax, IM_COL32(20, 24, 36, 220), 12.0f);
                drawList->AddRect(pausedMin, pausedMax, IM_COL32(120, 150, 255, 200), 12.0f, ImDrawFlags_None, 1.5f);
                ImVec2 labelPos(center.x - labelSize.x * 0.5f, center.y - labelSize.y * 0.5f);
                drawList->AddText(labelPos, IM_COL32(240, 243, 255, 255), pausedLabel);
            }
        }
    }

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

DemoScene* GamePanel::GetScene()
{
    if (!sceneInitialized) {
        return nullptr;
    }
    return &demoScene;
}

void GamePanel::TogglePause()
{
    SetPaused(!paused);
}

void GamePanel::SetPaused(bool value)
{
    if (paused == value) {
        return;
    }

    paused = value;

    if (paused) {
        if (capturingInput && windowOwner) {
            windowOwner->SetCursorMode(CursorMode::Normal);
        }
        capturingInput = false;
    } else {
        lastFrameTime = glfwGetTime();
    }
}

bool GamePanel::IsPaused()
{
    return paused;
}

void GamePanel::Restart()
{
    if (!sceneInitialized) {
        return;
    }

    paused = false;
    capturingInput = false;
    if (windowOwner) {
        windowOwner->SetCursorMode(CursorMode::Normal);
    }

    demoScene.ResetGameplay();
    lastFrameTime = glfwGetTime();
}

void GamePanel::Reload()
{
    if (!sceneInitialized) {
        return;
    }

    paused = false;
    capturingInput = false;
    if (windowOwner) {
        windowOwner->SetCursorMode(CursorMode::Normal);
    }

    demoScene.Reload();
    lastFrameTime = glfwGetTime();
}
