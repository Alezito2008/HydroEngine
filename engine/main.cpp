#include "Window/Window.h"
#include "Graphics/GraphicsContext.h"
#include "Rendering/Renderer.h"
#include "Graphics/Shader.h"
#include "Graphics/Texture.h"
#include "Core/Input.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

int main() {
    try {
        // Crear ventana
        Hydro::Window window(800, 600, "HydroEngine - Renderer Test");

        // Crear contexto gráfico
        Hydro::GraphicsContext context(window.GetWindowObject());
        context.Init();

        // Inicializar el sistema de input
        Hydro::Input::Init(window.GetWindowObject());

        // Inicializar el renderer
        Hydro::Renderer::Init();

        // Crear shader básico para texturas
        Hydro::Shader textureShader("resources/basic_vertex.glsl", "resources/basic_fragment.glsl");

        // Variables para movimiento interactivo
        glm::vec2 playerPos(0.0f, 0.0f);
        float playerSpeed = 3.0f;
        float deltaTime = 0.0f;
        float lastFrame = 0.0f;

        // Configurar matrices de vista y proyección
        glm::mat4 projection = glm::ortho(-4.0f, 4.0f, -3.0f, 3.0f, -1.0f, 1.0f);
        glm::mat4 view = glm::mat4(1.0f);
        glm::mat4 viewProjection = projection * view;

        std::cout << "Motor iniciado! Usa WASD para mover el cuad rojo, ESC para salir." << std::endl;

        // Bucle principal
        while (!window.ShouldClose()) {
            // Calcular delta time
            float currentFrame = static_cast<float>(glfwGetTime());
            deltaTime = currentFrame - lastFrame;
            lastFrame = currentFrame;

            // Actualizar sistema de input
            Hydro::Input::Update();

            // Procesar eventos
            Hydro::Window::PollEvents();

            // Manejo de input
            if (Hydro::Input::IsKeyPressed(Hydro::KeyCode::Escape)) {
                break;
            }

            // Movimiento del jugador con WASD
            if (Hydro::Input::IsKeyPressed(Hydro::KeyCode::W)) {
                playerPos.y += playerSpeed * deltaTime;
            }
            if (Hydro::Input::IsKeyPressed(Hydro::KeyCode::S)) {
                playerPos.y -= playerSpeed * deltaTime;
            }
            if (Hydro::Input::IsKeyPressed(Hydro::KeyCode::A)) {
                playerPos.x -= playerSpeed * deltaTime;
            }
            if (Hydro::Input::IsKeyPressed(Hydro::KeyCode::D)) {
                playerPos.x += playerSpeed * deltaTime;
            }

            // Debug: mostrar posición cuando se presiona space
            if (Hydro::Input::IsKeyJustPressed(Hydro::KeyCode::Space)) {
                std::cout << "Posición del jugador: (" << playerPos.x << ", " << playerPos.y << ")" << std::endl;
            }

            // Limpiar pantalla
            Hydro::Renderer::Clear(0.2f, 0.3f, 0.3f, 1.0f);

            // Comenzar escena
            Hydro::Renderer::BeginScene(viewProjection);

            // Dibujar jugador (quad rojo que se mueve)
            Hydro::Renderer::DrawQuad(
                playerPos,                           // Posición controlada por input
                glm::vec2(0.5f, 0.5f),              // Tamaño
                glm::vec4(1.0f, 0.0f, 0.0f, 1.0f)   // Color rojo
            );

            // Dibujar quad estático de referencia
            Hydro::Renderer::DrawQuad(
                glm::vec2(2.0f, 0.0f),               // Posición fija
                glm::vec2(0.8f, 0.8f),               // Tamaño
                glm::vec4(0.0f, 1.0f, 0.0f, 1.0f)   // Color verde
            );

            // Dibujar cursor del mouse (quad pequeño)
            auto mousePos = Hydro::Input::GetMousePosition();
            // Convertir coordenadas de pantalla a coordenadas del mundo
            float worldMouseX = (mousePos.first / 800.0f * 8.0f) - 4.0f;
            float worldMouseY = -((mousePos.second / 600.0f * 6.0f) - 3.0f);

            Hydro::Renderer::DrawQuad(
                glm::vec2(worldMouseX, worldMouseY), // Posición del mouse
                glm::vec2(0.1f, 0.1f),               // Tamaño pequeño
                glm::vec4(1.0f, 1.0f, 0.0f, 1.0f)   // Color amarillo
            );

            // Finalizar escena
            Hydro::Renderer::EndScene();

            // Intercambiar buffers
            context.SwapBuffers();
        }

        // Limpiar recursos
        Hydro::Renderer::Shutdown();

        std::cout << "Aplicación finalizada correctamente" << std::endl;

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return -1;
    }

    return 0;
}