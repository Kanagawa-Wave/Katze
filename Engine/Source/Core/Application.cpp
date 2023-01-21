#include "pch.h"

#include "Application.h"

#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>
#include <imgui.h>
#include <UI/imgui_impl_glfw.h>
#include <UI/imgui_impl_opengl3.h>
#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Core.h"


Application* Application::s_pApplication = nullptr;

Application::~Application()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

Application* Application::GetInstance()
{
    if (s_pApplication == nullptr)
        s_pApplication = new Application();
    return s_pApplication;
}

void Application::Init()
{
    Log::Init();

    m_editor = new Editor();
    m_editor->SetEventCallback(BIND_EVENT_FN(Application::OnEvent));

    Input::Init(glfwGetWin32Window(GetWindow()->GetGLFWWindow()));

    Renderer::Init();
    m_scene.Init();
    m_editor->Init();
}

void Application::Update(float dt)
{
    // Input
    glfwPollEvents();

    m_scene.OnUpdate(dt);
}

void Application::Render()
{
    m_editor->StartViewportRender();
    Renderer::SetClearColor({0.0f, 0.0f, 0.0f, 1.0f});
    Renderer::Clear();
    Renderer::Submit(m_scene);
    m_editor->EndViewportRender();
}

void Application::Run()
{
    Timer timer;
    while (m_isRunning)
    {
        if (!m_isMinimized)
        {
            float dt = timer.Elapsed();
            timer.Reset();
            Update(dt);
            m_editor->Render(m_scene);
            Render();
        }

        m_editor->OnUpdate();
    }
}

void Application::OnEvent(Event& e)
{
    EventDispatcher dispatcher(e);
    dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(Application::OnWindowClose));
    dispatcher.Dispatch<WindowResizeEvent>(BIND_EVENT_FN(Application::OnWindowResize));

    m_scene.camera->OnEvent(e);
    m_editor->OnEvent(e);
}

void Application::LoadDemo()
{
    GameObject AVase1 = GameObject::CreateGameObject();
    AVase1.AddMesh("Assets/smooth_vase.obj", "Shaders/triangle", "Assets/white.png");
    AVase1.SetRotationZ(glm::pi<float>());
    AVase1.SetScale({3.f, 3.f, 3.f});
    AVase1.Translate({0.5f, -0.5f, 0.f});
    m_scene.Objects.emplace(AVase1.GetID(), std::move(AVase1));

    GameObject AVase2 = GameObject::CreateGameObject();
    AVase2.AddMesh("Assets/flat_vase.obj", "Shaders/triangle", "Assets/white.png");
    AVase2.SetRotationZ(glm::pi<float>());
    AVase2.SetScale({3.f, 3.f, 3.f});
    AVase2.Translate({-0.5f, -0.5f, 0.f});
    m_scene.Objects.emplace(AVase2.GetID(), std::move(AVase2));

    GameObject AFloor = GameObject::CreateGameObject();
    AFloor.AddMesh("Assets/floor.obj", "Shaders/triangle", "Assets/white.png");
    AFloor.SetRotationZ(glm::pi<float>());
    AFloor.Translate({0.f, -0.5f, 0.0f});
    AFloor.SetScale({3.f, 1.f, 3.f});
    m_scene.Objects.emplace(AFloor.GetID(), std::move(AFloor));

    m_scene.uboData.ambientColor = {1.f, 1.f, 1.f, 0.05f};
    m_scene.uboData.lightColor = {1.f, 1.f, 1.f, 1.f};
}

bool Application::OnWindowClose(WindowCloseEvent& e)
{
    m_isRunning = false;
    return true;
}

bool Application::OnWindowResize(WindowResizeEvent& e)
{
    if (e.GetWidth() == 0 || e.GetHeight() == 0)
    {
        m_isMinimized = true;
        return false;
    }

    m_isMinimized = false;
    Renderer::ResizeViewport(e.GetWidth(), e.GetHeight());

    return true;
}
