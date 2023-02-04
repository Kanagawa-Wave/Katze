#pragma once

#include <Katze.h>

class EditorLayer : public Layer
{
public:
    EditorLayer();
    virtual ~EditorLayer() override = default;

    virtual void OnAttach() override;
    virtual void OnDetach() override;

    virtual void OnEvent(Event& event) override;
    virtual void OnUpdate(Timestep& ts) override;
    virtual void OnImGuiRender() override;

private:
    void DockSpace();
    void Viewport();
    void EditorPanels();
    void StatsOverlay();
    
    bool OnKeyPressed(KeyPressedEvent& e);
    bool OnMouseButtonPressed(MouseButtonPressedEvent& e);

    Framebuffer* m_Framebuffer = nullptr;
    Camera* m_EditorCamera = nullptr;
    Scene* m_Scene = nullptr;
    Outliner* m_Outliner = nullptr;

    Timestep m_TS;

    // TODO: Remove
    Shader* m_testCS = nullptr;
    Texture2D* m_testOutput = nullptr;
};
