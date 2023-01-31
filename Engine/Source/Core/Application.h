#pragma once

#include "Window.h"

#include "Event/Event.h"
#include "Event/ApplicationEvent.h"
#include "Layer/LayerStack.h"
#include "UI/ImGuiLayer.h"

/*! @brief Application class maintains the OpenGL context and currently owns a GLFW window.
 *
 *  Currently a singleton class.
 * 
 */
class Application
{
public:
    Application();
    ~Application();
    
    Application(const Application& app) = delete;
    Application& operator=(const Application&) = delete;
    
    inline Window* GetWindow() { return m_Window; }
    static inline Application* Get() { return s_Application; }
    
    void Run();
    void OnEvent(Event& e);

    void PushLayer(Layer* layer);
    void PushOverlay(Layer* layer);
private:
    bool OnWindowClose(WindowCloseEvent& e);
    bool OnWindowResize(WindowResizeEvent& e);

    LayerStack m_LayerStack;
    
    Window* m_Window = nullptr;
    WindowProps m_Props = WindowProps("Katze", 1920, 1080);

    ImGuiLayer* m_ImGuiLayer = nullptr;
    static Application* s_Application;

    bool m_Running = true;
    bool m_Minimized = false;
};

Application* CreateApplication();
