#include "pch.h"

#include "Input.h"

#include "Application.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

Input* Input::s_instance = new Input();

void Input::Init(HWND hwnd)
{
    HIMC hIMC = NULL;
    hIMC = ImmAssociateContext(hwnd, NULL);
}

bool Input::IsKeyPressed(int keycode)
{
    GLFWwindow* window = static_cast<GLFWwindow*>(Application::GetInstance()->GetWindow()->GetNativeWindow());
    const int state = glfwGetKey(window, keycode);
    return state == GLFW_PRESS || state == GLFW_REPEAT;
}

bool Input::IsMouseButtonPressed(int button)
{
    GLFWwindow* window = static_cast<GLFWwindow*>(Application::GetInstance()->GetWindow()->GetNativeWindow());
    const int state = glfwGetMouseButton(window, button);
    return state == GLFW_PRESS;
}

float Input::GetMouseX()
{
    return GetMousePosition().x;
}

float Input::GetMouseY()
{
    return GetMousePosition().y;
}

glm::vec2 Input::GetMousePosition()
{
    GLFWwindow* window = static_cast<GLFWwindow*>(Application::GetInstance()->GetWindow()->GetNativeWindow());
    double x, y;
    glfwGetCursorPos(window, &x, &y);
    return {x, y};
}

void Input::SetCursorMode(CursorMode mode)
{
    GLFWwindow* window = static_cast<GLFWwindow*>(Application::GetInstance()->GetWindow()->GetNativeWindow());
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL + (int)mode);
}
