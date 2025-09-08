#pragma once
#include "Renderer.h"
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "../Graphics/Shader.h"
#include "../Graphics/Texture.h"
#include <iostream>
#include <memory>


namespace Hydro{

    unsigned int Renderer::s_QuadVAO = 0;
    unsigned int Renderer::s_QuadVBO = 0;
    unsigned int Renderer::s_QuadEBO = 0;
    bool Renderer::s_Initialized = false;
    glm::mat4 Renderer::s_ViewProjectionMatrix = glm::mat4(1.0f);
    std::unique_ptr<Shader> Renderer::s_ColorShader = nullptr;

    void Renderer::Init()
    {
        if (s_Initialized)
            return;

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glEnable(GL_DEPTH_TEST);

        float quadVertices[] = {

            -0.5f, -0.5f, 0.0f,  0.0f, 0.0f,
             0.5f, -0.5f, 0.0f,  1.0f, 0.0f,
             0.5f,  0.5f, 0.0f,  1.0f, 1.0f,
            -0.5f,  0.5f, 0.0f,  0.0f, 1.0f
        };

        unsigned int indices[] = {
            0, 1, 2,   // Primer triángulo
            2, 3, 0    // Segundo triángulo
        };

        // Generar y configurar VAO, VBO y EBO
        glGenVertexArrays(1, &s_QuadVAO);
        glGenBuffers(1, &s_QuadVBO);
        glGenBuffers(1, &s_QuadEBO);

        glBindVertexArray(s_QuadVAO);

        glBindBuffer(GL_ARRAY_BUFFER, s_QuadVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, s_QuadEBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), nullptr);
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        glBindVertexArray(0);

        try {
            s_ColorShader = std::make_unique<Shader>("resources/color_vertex.glsl", "resources/color_fragment.glsl");
        } catch (const std::exception& e) {
            std::cerr << "Error al cargar shader de colores: " << e.what() << std::endl;
        }

        s_Initialized = true;
        std::cout << "Renderer inicializado correctamente" << std::endl;
    }

    void Renderer::Shutdown()
    {
        if (!s_Initialized)
            return;

        if (s_QuadVAO) {
            glDeleteVertexArrays(1, &s_QuadVAO);
            s_QuadVAO = 0;
        }
        if (s_QuadVBO) {
            glDeleteBuffers(1, &s_QuadVBO);
            s_QuadVBO = 0;
        }
        if (s_QuadEBO) {
            glDeleteBuffers(1, &s_QuadEBO);
            s_QuadEBO = 0;
        }

        s_ColorShader.reset();

        s_Initialized = false;
        std::cout << "Renderer finalizado" << std::endl;
    }

    void Renderer::Clear(float r, float g, float b, float a)
    {
        glClearColor(r, g, b, a);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void Renderer::BeginScene(const glm::mat4& viewProjection)
    {
        s_ViewProjectionMatrix = viewProjection;
    }

    void Renderer::EndScene()
    {
       // PLACEHOLDER
    }

    void Renderer::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color)
    {
        if (!s_Initialized || !s_ColorShader) {
            std::cerr << "Error: Renderer no está inicializado o shader de colores no está disponible" << std::endl;
            return;
        }

        auto transform = glm::mat4(1.0f);
        transform = glm::translate(transform, glm::vec3(position.x, position.y, 0.0f));
        transform = glm::scale(transform, glm::vec3(size.x, size.y, 1.0f));

        auto mvp = s_ViewProjectionMatrix * transform;

        s_ColorShader->use();
        s_ColorShader->setMat4("u_MVP", mvp);
        s_ColorShader->setVec4("u_Color", color);

        glBindVertexArray(s_QuadVAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
        glBindVertexArray(0);
    }

    void Renderer::DrawQuad(const glm::mat4& transform, const Shader& shader, const Texture& texture)
    {
        if (!s_Initialized) {
            std::cerr << "Error: Renderer no está inicializado" << std::endl;
            return;
        }

        shader.use();

        auto mvp = s_ViewProjectionMatrix * transform;

        shader.setMat4("u_MVP", mvp);
        shader.setMat4("u_Transform", transform);
        shader.setMat4("u_ViewProjection", s_ViewProjectionMatrix);

        texture.Bind(0);
        shader.setInt("u_Texture", 0);

        glBindVertexArray(s_QuadVAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
        glBindVertexArray(0);

        glBindTexture(GL_TEXTURE_2D, 0);
    }
}
