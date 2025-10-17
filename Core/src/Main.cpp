#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb/stb_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "WindowManager.h"
#include "Shader.h"
#include "Renderer.h"
#include "Camera.h"
#include "InputManager.h"
#include "Model.h"

WindowSettings windowSettings("OpenGL Model Loader", 1500, 1000);
WindowManager windowManager(windowSettings);
GLFWwindow* window = windowManager.GetWindow();

float pitch = 0.0f;
float yaw = -90.0f;
float lastX = windowSettings.width / 2.0f;
float lastY = windowSettings.height / 2.0f;

static void processMouse(double xpos, double ypos) {
    const float sensitivity = 0.1f;
    float deltaX = (xpos - lastX) * sensitivity;
    float deltaY = (lastY - ypos) * sensitivity; // invertido Y
    lastX = xpos;
    lastY = ypos;
    yaw += deltaX;
    pitch += deltaY;
    if (pitch > 89.0f) pitch = 89.0f;
    if (pitch < -89.0f) pitch = -89.0f;
}

static void processInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

int main() {
    glEnable(GL_DEPTH_TEST);
    Renderer renderer;
    Camera camera;
    InputManager inputManager(window);
    Shader shaderModel("res/shaders/model.vert", "res/shaders/model.frag");

    // ###########
    // # MODELO  #
    // ###########
    Model backpack("src/Model/backpack.obj");

    windowManager.SetCursorMode(CursorMode::Disabled);
    windowManager.SetCursorPosCallback(processMouse);

    glm::mat4 projection = glm::perspective(glm::radians(75.0f), windowManager.GetAspectRatio(), 0.1f, 100.0f);
    camera.SetPosition(glm::vec3(0.0f, 0.0f, 3.0f));

    glm::vec3 lightPos(2.0f, 2.0f, 2.0f);
    glm::vec3 lightColor(1.0f);

    while (!glfwWindowShouldClose(window)) {
        processInput(window);
        renderer.Clear();

        glm::vec3 front;
        front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        front.y = sin(glm::radians(pitch));
        front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
        camera.SetFront(front);

        glm::vec3 direction(0.0f);
        const float speed = 0.05f;
        if (inputManager.IsKeyPressed(KeyCode::KEY_W)) direction += camera.GetFront();
        if (inputManager.IsKeyPressed(KeyCode::KEY_S)) direction -= camera.GetFront();
        if (inputManager.IsKeyPressed(KeyCode::KEY_A)) direction -= camera.GetRight();
        if (inputManager.IsKeyPressed(KeyCode::KEY_D)) direction += camera.GetRight();
        if (glm::length(direction) > 0) camera.Move(glm::normalize(direction) * speed);

        glm::mat4 view = camera.GetView();

        shaderModel.Bind();
        shaderModel.setVec3("viewPos", camera.GetPosition());
        shaderModel.setVec3("lightPos", lightPos);
        shaderModel.setVec3("lightColor", lightColor);

        glm::mat4 model(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, -1.5f, 0.0f));
        model = glm::scale(model, glm::vec3(1.0f));

        shaderModel.setMVP(model, view, projection);
        backpack.Draw(shaderModel, renderer);

        windowManager.PollEventsAndSwapBuffers();
    }

    return 0;
}
