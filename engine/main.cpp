#include "Window/Window.h"
#include "Graphics/GraphicsContext.h"
#include "Rendering/Renderer.h"
#include "Graphics/Shader.h"
#include "Graphics/Texture.h"
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

        // Inicializar el renderer
        Hydro::Renderer::Init();

        // Crear shader básico para texturas
        Hydro::Shader textureShader("resources/basic_vertex.glsl", "resources/basic_fragment.glsl");

        // Crear una textura de prueba (necesitarás una imagen en resources/)
        // Por ahora comentamos esto hasta tener una imagen
        // Hydro::Texture testTexture("resources/test_texture.png");

        // Configurar matrices de vista y proyección
        glm::mat4 projection = glm::ortho(-4.0f, 4.0f, -3.0f, 3.0f, -1.0f, 1.0f);
        glm::mat4 view = glm::mat4(1.0f);
        glm::mat4 viewProjection = projection * view;


        // Bucle principal
        while (!window.ShouldClose()) {
            // Procesar eventos
            Hydro::Window::PollEvents();

            // Limpiar pantalla
            Hydro::Renderer::Clear(0.2f, 0.3f, 0.3f, 1.0f);

            // Comenzar escena
            Hydro::Renderer::BeginScene(viewProjection);

            // Dibujar quad con color sólido (posición, tamaño, color)
            Hydro::Renderer::DrawQuad(
                glm::vec2(0.0f, 0.0f),      // Posición centro
                glm::vec2(1.0f, 1.0f),      // Tamaño
                glm::vec4(1.0f, 0.0f, 0.0f, 1.0f)  // Color rojo
            );

            // Dibujar otro quad desplazado
            Hydro::Renderer::DrawQuad(
                glm::vec2(2.0f, 0.0f),      // Posición derecha
                glm::vec2(0.8f, 0.8f),      // Tamaño
                glm::vec4(0.0f, 1.0f, 0.0f, 1.0f)  // Color verde
            );

            // Dibujar quad con transformación manual (si tienes textura)
            /*
            glm::mat4 transform = glm::mat4(1.0f);
            transform = glm::translate(transform, glm::vec3(-2.0f, 0.0f, 0.0f));
            transform = glm::scale(transform, glm::vec3(1.2f, 1.2f, 1.0f));
            Hydro::Renderer::DrawQuad(transform, textureShader, testTexture);
            */

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