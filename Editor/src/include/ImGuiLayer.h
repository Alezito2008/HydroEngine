#pragma once

class GLFWwindow;
class ImGuiIO;

class ImGuiLayer {
public:
    ImGuiLayer(GLFWwindow* window);
    ~ImGuiLayer();
    void StartFrame();
    void EndFrame();
private:
    ImGuiIO* io = nullptr;
};
