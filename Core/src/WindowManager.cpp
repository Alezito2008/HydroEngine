#include "WindowManager.h"

#include <algorithm>
#include <iostream>

bool WindowManager::m_IsMinimized = false;

void WindowManager::FrameBufferSizeCallback(GLFWwindow* window, int width, int height)
{
	m_IsMinimized = (width == 0 || height == 0);
	WindowManager* wm = static_cast<WindowManager*>(glfwGetWindowUserPointer(window));
	if (wm) {
		wm->SetDimensions(width, height);
	}
	glViewport(0, 0, width, height);
}

void WindowManager::CursorPosCallbackInternal(GLFWwindow* window, double posX, double posY)
{
	WindowManager* wm = static_cast<WindowManager*>(glfwGetWindowUserPointer(window));
	if (wm && wm->m_cursorPosCallback) {
		wm->m_cursorPosCallback(posX, posY);
	}
}

WindowManager::WindowManager(const WindowSettings& settings) : m_windowSettings(settings)
{

	if (!glfwInit()) {
		std::cout << "Error: No se pudo inicializar GLFW" << std::endl;
		std::exit(-1);
	};

	// Decirle a OpenGL los datos sobre la versi�n
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Crear ventana
	m_window = glfwCreateWindow(m_windowSettings.width, m_windowSettings.height, m_windowSettings.name.c_str(), NULL, NULL);
	// Si no se pudo crear, terminar
	if (m_window == NULL) {
		std::cout << "Error: No se pudo crear la ventana" << std::endl;
		glfwTerminate();
		std::exit(-1);
	}

	glfwSetWindowUserPointer(m_window, this);
	glfwMakeContextCurrent(m_window);
	glfwSwapInterval(1);
	m_vsyncEnabled = true;
	glfwGetWindowPos(m_window, &m_windowedPosX, &m_windowedPosY);
	glfwGetWindowSize(m_window, &m_windowedWidth, &m_windowedHeight);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Error al inicializar GLAD" << std::endl;
		std::exit(-1);
	}

	glViewport(0, 0, m_windowSettings.width, m_windowSettings.height);

	// Handlear cambios de tama�o
	glfwSetFramebufferSizeCallback(m_window, FrameBufferSizeCallback);
}

WindowManager::~WindowManager()
{
	// Eliminar ventana antes de terminar
	glfwDestroyWindow(m_window);
	// Terminar OpenGL
	glfwTerminate();
}

const WindowSettings& WindowManager::GetSettings() const
{
	return m_windowSettings;
}

void WindowManager::PollEventsAndSwapBuffers()
{
	if (m_IsMinimized) {
		glfwWaitEvents();
		return;
	}
	glfwSwapBuffers(m_window);
	glfwPollEvents();
}

void WindowManager::SetDimensions(int width, int height)
{
	m_windowSettings.width = width;
	m_windowSettings.height = height;
	glViewport(0, 0, width, height);
	if (!m_isFullscreen) {
		m_windowedWidth = width;
		m_windowedHeight = height;
	}
}

void WindowManager::SetCursorMode(CursorMode mode)
{
	glfwSetInputMode(m_window, GLFW_CURSOR, static_cast<int>(mode));
}

void WindowManager::SetCursorPosCallback(CursorPosCallback handler)
{
	m_cursorPosCallback = handler;
	glfwSetCursorPosCallback(m_window, CursorPosCallbackInternal);
}

void WindowManager::CloseWindow()
{
	glfwSetWindowShouldClose(m_window, true);
}

float WindowManager::GetAspectRatio() const
{
	return static_cast<float>(m_windowSettings.width) / m_windowSettings.height;
}

bool WindowManager::GetWindowShouldClose() const
{
	return glfwWindowShouldClose(m_window);
}

CursorMode WindowManager::GetCursorMode() const
{
	int mode = glfwGetInputMode(m_window, GLFW_CURSOR);
	return static_cast<CursorMode>(mode);
}

void WindowManager::SetFullscreen(bool enabled)
{
	if (m_isFullscreen == enabled) {
		return;
	}

	if (enabled) {
		glfwGetWindowPos(m_window, &m_windowedPosX, &m_windowedPosY);
		glfwGetWindowSize(m_window, &m_windowedWidth, &m_windowedHeight);

		GLFWmonitor* monitor = glfwGetPrimaryMonitor();
		if (!monitor) {
			return;
		}
		const GLFWvidmode* mode = glfwGetVideoMode(monitor);
		if (!mode) {
			return;
		}

		glfwSetWindowMonitor(m_window, monitor, 0, 0, mode->width, mode->height, mode->refreshRate);
		m_isFullscreen = true;
	} else {
		glfwSetWindowMonitor(m_window, nullptr, m_windowedPosX, m_windowedPosY, std::max(m_windowedWidth, 1), std::max(m_windowedHeight, 1), 0);
		m_isFullscreen = false;
	}

	if (m_isFullscreen) {
		const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
		if (mode) {
			m_windowSettings.width = mode->width;
			m_windowSettings.height = mode->height;
			glViewport(0, 0, mode->width, mode->height);
		}
	} else {
		m_windowSettings.width = std::max(m_windowedWidth, 1);
		m_windowSettings.height = std::max(m_windowedHeight, 1);
		glViewport(0, 0, m_windowSettings.width, m_windowSettings.height);
	}
}

void WindowManager::ToggleFullscreen()
{
	SetFullscreen(!m_isFullscreen);
}

bool WindowManager::IsFullscreen() const
{
	return m_isFullscreen;
}

void WindowManager::SetVSync(bool enabled)
{
	m_vsyncEnabled = enabled;
	glfwSwapInterval(enabled ? 1 : 0);
}

void WindowManager::ToggleVSync()
{
	SetVSync(!m_vsyncEnabled);
}

bool WindowManager::IsVSyncEnabled() const
{
	return m_vsyncEnabled;
}
