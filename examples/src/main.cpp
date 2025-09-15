#include "Window/Window.h"
#include "core/Input.h"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "Graphics/Camera.h"
#include "Graphics/GraphicsContext.h"
#include "Graphics/IndexBuffer.h"
#include "Graphics/Shader.h"
#include "Graphics/Texture.h"
#include "Graphics/VertexArray.h"
#include "Rendering/Renderer.h"



static void processMouse(double xpos, double ypos) {
    const float sensitivity = 0.2f;

    float deltaX = (xpos - lastX) * sensitivity;
    float deltaY = (ypos - lastY) * sensitivity;

    lastX = xpos;
    lastY = ypos;

    yaw += deltaX;
    pitch += deltaY;

    if (pitch > 89.0f) {
        pitch = 89.0f;
    }
    else if (pitch < -89.0f) {
        pitch = -89.0f;
    }
}

static void processInput(Hydro::Window window, Hydro::Input InputManager) {
    if (InputManager.IsKeyPressed(Hydro::KeyCode::Escape))
        window.SetShouldClose(true);
}


int main()
{
	Hydro::Window window(800, 600, "Prueba");

	float pitch = 0.0f;
	float yaw = 0.0f;

	float lastX = window.GetWidth() / 2;
	float lastY = window.GetHeight() / 2;


	Hydro::Shader shaderTextura = Hydro::Shader("texture.vert", "texture.frag");


	float vertices[] = {
		-0.5f, -0.5f, /* abajo izquierda */ 0.0f, 0.0f,
		0.5f, -0.5f, /* abajo derecha* */ 1.0f, 0.0f,
		0.5f, 0.5f, /* arriba derecha */ 1.0f, 1.0f,
		-0.5f, 0.5f, /* arriba izquierda */ 0.0f, 1.0f
	};

	unsigned int indices[] = {
		0, 1, 2, // primer triangulo (arriba derecha)
		2, 3, 0, // segundo triangulo (abajo izquierda)
	};

	float cubeVertices[] = {
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
		 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
		};

	glm::vec3 cubePositions[] = {
		glm::vec3(0.0f,  0.0f,  0.0f),
		glm::vec3(2.0f,  5.0f, -15.0f),
		glm::vec3(-1.5f, -2.2f, -2.5f),
		glm::vec3(-3.8f, -2.0f, -12.3f),
		glm::vec3(2.4f, -0.4f, -3.5f),
		glm::vec3(-1.7f,  3.0f, -7.5f),
		glm::vec3(1.3f, -2.0f, -2.5f),
		glm::vec3(1.5f,  2.0f, -2.5f),
		glm::vec3(1.5f,  0.2f, -1.5f),
		glm::vec3(-1.3f,  1.0f, -1.5f)
		};

	Hydro::Texture containerTexture("container.jpg");
	Hydro::Texture faceTexture("awesomeface.png");

	Hydro::Renderer renderer;
	Hydro::Camera camera;
	Hydro::Input InputManager;
	InputManager.Init(window);

	Hydro::GraphicsContext graphicscontext;
	graphicscontext.Init();

	Hydro::VertexArray va;
	va.Bind();
	Hydro::VertexBuffer VBO(vertices, sizeof(vertices));
	Hydro::IndexBuffer EBO(indices, 6);
	Hydro::VertexBufferLayout layout;
	layout.Push<float>(2, false);
	layout.Push<float>(2, false);
	va.AddBuffer(VBO, layout);

	Hydro::VertexArray cubeVA;
	Hydro::VertexBuffer cubeVBO(cubeVertices, sizeof(cubeVertices));
	Hydro::VertexBufferLayout cubeLayout;
	cubeLayout.Push<float>(3, false); // posiciones
	cubeLayout.Push<float>(2, false); // UV
	cubeVA.AddBuffer(cubeVBO, cubeLayout);

	shaderTextura.use();

	shaderTextura.setInt("texture1", 0);
	shaderTextura.setInt("texture2", 1);

	containerTexture.Bind();
	faceTexture.Bind(1);
	glClearColor(0.0, 0.0, 0.5, 1.0);


	glm::mat4 projection = glm::mat4(1.0f);
	projection = glm::perspective(glm::radians(80.0f), static_cast<float>(window.GetHeight() / window.GetWidth() ), 0.1f, 100.0f);

	camera.SetPosition(glm::vec3(0.0f, 0.0f, -3.0f));



    while (!window.GetShouldClose())
    {
	    InputManager.Update();
    	renderer.Clear();
    	glm::vec3 direction(0.0f);
    	const float cameraSpeed = 0.05f;
    	if (InputManager.IsKeyPressed(Hydro::KeyCode::W)) {
    		direction += camera.GetFront();
    	}
    	if (InputManager.IsKeyPressed(Hydro::KeyCode::S)) {
    		direction -= camera.GetFront();
    	}
    	if (InputManager.IsKeyPressed(Hydro::KeyCode::A)) {
    		direction += camera.GetRight();
    	}
    	if (InputManager.IsKeyPressed(Hydro::KeyCode::D)) {
    		direction -= camera.GetRight();
    	}
    	glm::vec3 front;
    	front.x = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    	front.y = sin(glm::radians(pitch));
    	front.z = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    	camera.SetFront(front);

    	if (glm::length(direction) > 0.0f) {
    		direction = glm::normalize(direction);
    		camera.Move(direction * cameraSpeed);
    	}

    	for (glm::vec3 pos : cubePositions) {
    		float rotation = 45.0f * static_cast<float>(glfwGetTime()) * 0.001f;

    		glm::mat4 model = glm::mat4(1.0f);
    		model = glm::translate(model, pos);
    		model = glm::rotate(model, glm::degrees(rotation + pos.x), glm::vec3(0.3f, 1.0f, 0.0f));

    		shaderTextura.setMat4("transform", (projection * camera.GetView() * model));

    		renderer.Draw(cubeVA, shaderTextura, 36);
    	}
		graphicscontext.SwapBuffers();
    	window.PollEvents();
    }

    return 0;
}