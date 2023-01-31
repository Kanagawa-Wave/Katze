#include "pch.h"

#include "Application.h"

#include "Timer.h"
#include "Renderer/Renderer.h"

#define APP_BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)

Application* Application::s_Application = nullptr;

Application::Application()
{
    s_Application = this;
    
    Log::Init();
    
    m_Window = new Window(m_Props);

    Renderer::Init();
    
    m_ImGuiLayer = new ImGuiLayer();
    m_Window->SetEventCallback(APP_BIND_EVENT_FN(OnEvent));
    m_Window->SetVSync(false);
    PushOverlay(m_ImGuiLayer);
}

Application::~Application() = default;

void Application::Run()
{
    Timer timer;
    while (m_Running)
    {
        if (!m_Minimized)
        {
            Timestep ts = timer.Elapsed();
            timer.Reset();

            for (Layer* layer : m_LayerStack)
                layer->OnUpdate(ts);

            m_ImGuiLayer->Begin();
            for (Layer* layer : m_LayerStack)
                layer->OnImGuiRender();
            m_ImGuiLayer->End();

            m_Window->Update();
        }
    }
}

void Application::OnEvent(Event& e)
{
    EventDispatcher dispatcher(e);
    dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(Application::OnWindowClose));
    dispatcher.Dispatch<WindowResizeEvent>(BIND_EVENT_FN(Application::OnWindowResize));

    for (auto iterator = m_LayerStack.end(); iterator != m_LayerStack.begin();)
    {
        (*--iterator)->OnEvent(e);
        if (e.m_handled)
        {
            break;
        }
    }
}

void Application::PushLayer(Layer* layer)
{
    m_LayerStack.PushLayer(layer);
}

void Application::PushOverlay(Layer* layer)
{
    m_LayerStack.PushOverlay(layer);
    layer->OnAttach();
}


bool Application::OnWindowClose(WindowCloseEvent& e)
{
    m_Running = false;
    return true;
}

bool Application::OnWindowResize(WindowResizeEvent& e)
{
    if (e.GetWidth() == 0 || e.GetHeight() == 0)
    {
        m_Minimized = true;
        return false;
    }

    m_Minimized = false;
    Renderer::ResizeViewport(e.GetWidth(), e.GetHeight());

    return true;
}

#undef APP_BIND_EVENT_FN
