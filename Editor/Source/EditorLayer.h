#pragma once

#include <Katze.h>

class EditorLayer : public Layer
{
public:
    EditorLayer();
    virtual ~EditorLayer() = default;

    virtual void OnAttach() override;
    virtual void OnDetach() override;

    virtual void OnEvent(Event& event) override;
    virtual void OnUpdate(Timestep& ts) override;
    virtual void OnImGuiRender() override;

private:
    bool OnKeyPressed(KeyPressedEvent& e);
    bool OnMouseButtonPressed(MouseButtonPressedEvent& e);

    Framebuffer* m_Framebuffer = nullptr;
    Camera* m_EditorCamera = nullptr;
};
