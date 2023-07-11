#include "Window.h"

bool Window::InitGLFW()
{
	if (!glfwInit())
	{
		fprintf(stderr, "Failed to init GLFW\n");
		return false;
	}
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	printf("GLFW version: %s\n", glfwGetVersionString());

	m_Window = glfwCreateWindow(m_Data.width, m_Data.height, m_Data.title, nullptr, nullptr);

	if (!m_Window)
	{
		fprintf(stderr, "Failed to create GLFW window");
		return false;
	}

	return true;
}

void Window::SetGLFW()
{
	glfwSetWindowUserPointer(m_Window, &m_Data);
	glfwMakeContextCurrent(m_Window);
}

void Window::InitGLAD()
{
	if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress))) {
		fprintf(stderr, "Failed to init GLAD\n");
		return;
	}

	printf("OpenGL version: %s\n", glGetString(GL_VERSION));
}

Window::Window(const char* title, const int width, const int height, const bool vsync)
	: m_Window(nullptr), m_Data{title, width, height, vsync}
{
	if (!InitGLFW())
		return;

	SetGLFW();
	InitGLAD();

	SetVsync(m_Data.vsync);
}

Window::~Window()
{
	glfwDestroyWindow(m_Window);
	glfwTerminate();
}

void Window::Update()
{
	glfwSwapBuffers(m_Window);
	glfwPollEvents();
}

bool Window::ShouldClose()
{
	return glfwWindowShouldClose(m_Window);
}

bool Window::IsVsync()
{
	return m_Data.vsync;
}

void Window::SetVsync(bool vsync)
{
	m_Data.vsync = vsync;
	glfwSwapInterval(vsync ? 1 : 0);
}

GLFWwindow* Window::GetGlfwWindow()
{
	return m_Window;
}

WindowData* Window::GetWindowData()
{
	return &m_Data;
}

int Window::GetWidth()
{
	return m_Data.width;
}

int Window::GetHeight()
{
	return m_Data.height;
}
