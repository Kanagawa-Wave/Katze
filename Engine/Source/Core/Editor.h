#pragma once

#include "Event/Event.h"
#include "Window.h"
#include "Scene/Scene.h"

class Framebuffer;

class Editor
{
public:
    using EventCallbackFn = std::function<void(Event&)>;
    
    Editor();
    Editor(const Editor& app) = delete;
    ~Editor();
    
    void Init();
    void StartViewportRender();
    void EndViewportRender();
    void Render(Scene& scene);
    void OnUpdate() const;
    bool OnEvent(Event& e);
    void SetEventCallback(const EventCallbackFn& callback) const { m_window->SetEventCallback(callback); }

    const Window* GetWindow() const { return m_window; } 

private:
    void DockSpace();
    void Overlay(Scene& scene);

    Window* m_window = nullptr;
    Framebuffer* editorFrameBuffer;
    bool showNewGameObjectPrompt = false;
    char meshPath[128]{}, shaderPath[128]{}, texturePath[128]{};
};


