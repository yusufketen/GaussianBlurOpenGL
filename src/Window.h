#pragma once

#include "Engine.h"

struct WindowData
{
    const char *title;
    int width, height;
    bool vsync;
};

class Window
{
public:
    bool InitGLFW();
    void SetGLFW();
    void InitGLAD();

	Window(const char* title, const int width = 1280, const int height = 720, const bool vsync = true);
    ~Window();

    void Update();

    bool ShouldClose();

    bool IsVsync();
    void SetVsync(bool vsync);

    GLFWwindow* GetGlfwWindow();
    WindowData* GetWindowData();

	int GetWidth();
    int GetHeight();

private:
    GLFWwindow* m_Window;
    WindowData m_Data;
};
