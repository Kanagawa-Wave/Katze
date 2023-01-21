#include "pch.h"

#include "Context.h"

#include "Core/Core.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

Context::Context(GLFWwindow* windowHandle)
    : m_pWindowHandle(windowHandle)
{
    if (m_pWindowHandle == nullptr)
        LOG_ERROR("Window handle == nullptr")
}

void Context::Init()
{
    glfwMakeContextCurrent(m_pWindowHandle);

    const int status = gladLoadGLLoader(GLADloadproc(glfwGetProcAddress));
    if (status == 0)
        LOG_ERROR("Failed to initialize Glad")
    
    LOG_TRACE("OpenGL Device: ({0}) {1}", (const char*) glGetString(GL_VENDOR), (const char*) glGetString(GL_RENDERER))
    LOG_TRACE("OpenGL Driver: {0}", (const char*) glGetString(GL_VERSION))
}

void Context::SwapBuffers()
{
    glfwSwapBuffers(m_pWindowHandle);
}
