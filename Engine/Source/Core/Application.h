#pragma once

#include "Editor.h"
#include "Window.h"
#include "Renderer/Shader.h"
#include "Renderer/Buffers.h"
#include "Renderer/VertexArray.h"
#include "Renderer/Context.h"
#include "Renderer/Renderer.h"
#include "Renderer/Texture.h"

#include "Event/Event.h"
#include "Event/ApplicationEvent.h"
#include "Renderer/Framebuffer.h"
#include "Scene/Camera.h"
#include "Scene/GameObject.h"
#include "Scene/MeshComponent.h"

/*! @brief Application class maintains the OpenGL context and currently owns a GLFW window.
 *
 *  Currently a singleton class.
 * 
 */
class Application
{
public:
    Application(const Application& app) = delete;
    ~Application();

    static Application* GetInstance();
    const Window* GetWindow() const { return m_editor->GetWindow(); } 
    
    void Run();
    void OnEvent(Event& e);

    void Init();
    void LoadDemo();

private:
    void Update(float dt);
    void Render();
    
    Application() = default;
    bool OnWindowClose(WindowCloseEvent& e);
    bool OnWindowResize(WindowResizeEvent& e);
    
    static Application* s_pApplication;

    Editor* m_editor = nullptr;

    Scene m_scene;

    bool m_isRunning = true;
    bool m_isMinimized = false;
};
