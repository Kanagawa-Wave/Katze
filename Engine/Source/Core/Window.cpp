#include "pch.h"

#include "Window.h"

#include "Event/ApplicationEvent.h"
#include "Event/KeyEvent.h"
#include "Event/MouseEvent.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <shellscalingapi.h>

#include "Core.h"

static bool s_GLFWInitialized = false;

Window::Window(const WindowProps& props)
{
    Window::Init(props);
}

Window::~Window()
{
    Window::Shutdown();
}

void Window::Update()
{
    glfwPollEvents();
    glfwSwapBuffers(m_window);
}

void Window::SetVSync(bool enabled)
{
    glfwSwapInterval(enabled);
    m_data.VSync = enabled;
}

bool Window::IsVSync() const
{
    return m_data.VSync;
}

void Window::Init(const WindowProps& props)
{
    m_data.Width = props.Width;
    m_data.Height = props.Height;
    m_data.Title = props.Title;

    LOG_WARN("Creating window: {0} ({1}, {2})", props.Title, props.Width, props.Height);

    if (!s_GLFWInitialized)
    {
        // TODO: glfwTerminate on system shutdown
        int success = glfwInit();
        if(!success) 
            LOG_ERROR("Could not initialize GLFW!")

        s_GLFWInitialized = true;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

    m_window = glfwCreateWindow(static_cast<int>(props.Width), static_cast<int>(props.Height), m_data.Title.c_str(),
                                nullptr, nullptr);
    
    m_context = new Context(m_window);
    m_context->Init();

    glfwSetWindowUserPointer(m_window, &m_data);
    SetVSync(false);
    SetProcessDpiAwareness(PROCESS_PER_MONITOR_DPI_AWARE);

    // Set GLFW callbacks
    glfwSetWindowSizeCallback(m_window, [](GLFWwindow* window, int width, int height)
    {
        WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));

        WindowResizeEvent event(width, height);
        data.EventCallback(event);
        data.Width = width;
        data.Height = height;
    });

    glfwSetWindowCloseCallback(m_window, [](GLFWwindow* window)
    {
        WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));

        WindowCloseEvent event;
        data.EventCallback(event);
    });

    glfwSetKeyCallback(m_window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
    {
        WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));

        switch (action)
        {
        case GLFW_PRESS:
            {
                KeyPressedEvent event(key, 0);
                data.EventCallback(event);
                break;
            }
        case GLFW_REPEAT:
            {
                KeyPressedEvent event(key, 1);
                data.EventCallback(event);
                break;
            }
        case GLFW_RELEASE:
            {
                KeyReleasedEvent event(key);
                data.EventCallback(event);
                break;
            }
        }
    });

    glfwSetCursorPosCallback(m_window, [](GLFWwindow* window, double xPos, double yPos)
    {
        WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));

        MouseMovedEvent event(static_cast<float>(xPos), static_cast<float>(yPos));
        data.EventCallback(event);
    });

    glfwSetMouseButtonCallback(m_window, [](GLFWwindow* window, int button, int action, int mods)
    {
        WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));

        switch (action)
        {
        case GLFW_PRESS:
            {
                MouseButtonPressedEvent event(button);
                data.EventCallback(event);
                break;
            }
        case GLFW_RELEASE:
            {
                MouseButtonReleasedEvent event(button);
                data.EventCallback(event);
                break;
            }
        }
    });

    glfwSetScrollCallback(m_window, [](GLFWwindow* window, double Xoffset, double Yoffset)
    {
        WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));

        MouseScrolledEvent event(static_cast<float>(Xoffset), static_cast<float>(Yoffset));
        data.EventCallback(event);
    });

    glfwSetCharCallback(m_window, [](GLFWwindow* window, unsigned int keycode)
    {
        WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));

        KeyTypedEvent event(keycode);
        data.EventCallback(event);
    });

    glfwSetFramebufferSizeCallback(m_window, [](GLFWwindow* window, int width, int height)
    {
        WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));

        WindowResizeEvent event(width, height);
        data.EventCallback(event);
        data.Width = width;
        data.Height = height;
    });
}

void Window::Shutdown()
{
    glfwDestroyWindow(m_window);
    glfwTerminate();
}
