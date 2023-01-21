#pragma once

struct GLFWwindow;

class Context
{
public:
    Context(GLFWwindow* windowHandle);

    void Init();
    void SwapBuffers();

private:
    GLFWwindow* m_pWindowHandle = nullptr;
};
