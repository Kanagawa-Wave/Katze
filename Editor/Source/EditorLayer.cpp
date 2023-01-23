#include "EditorLayer.h"

#include <imgui/imgui.h>
#include <glm/gtc/type_ptr.hpp>

EditorLayer::EditorLayer()
    : Layer("EditorLayer")
{
}

void EditorLayer::OnAttach()
{
    m_Framebuffer = new Framebuffer();
    m_EditorCamera = new Camera(30.f, 0.001f, 1000.f);
}

void EditorLayer::OnDetach()
{
}

void EditorLayer::OnEvent(Event& event)
{
    Layer::OnEvent(event);
}

void EditorLayer::OnUpdate(Timestep& ts)
{
    Renderer::SetClearColor({0.0f, 0.2f, 0.2f, 0.8f});
    Renderer::Clear();
    
    m_Framebuffer->Bind();

    Renderer::SetClearColor({0.0f, 0.0f, 0.0f, 1.0f});
    Renderer::Clear();
    // Renderer::Submit(m_scene);
    
    m_Framebuffer->UnBind();
}

void EditorLayer::OnImGuiRender()
{
    ImGui::Begin("Config");
    ImGui::Text("Directional Light");
    // ImGui::SliderFloat3("Light Position", glm::value_ptr(scene.uboData.lightPosition), -5.f, 5.f);
    // ImGui::ColorEdit3("Light Color", glm::value_ptr(scene.uboData.lightColor));
    // ImGui::SliderFloat("Light Intensity", &scene.uboData.lightColor.w, 0.f, 1.f);
    ImGui::End();

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    ImGui::Begin("Viewport");
    {
        const uint32_t width = (uint32_t)ImGui::GetContentRegionAvail().x;
        const uint32_t height = (uint32_t)ImGui::GetContentRegionAvail().y;

        ImGui::Image(
            (ImTextureID)m_Framebuffer->GetTexture(),
            ImGui::GetContentRegionAvail(),
            ImVec2(0, 1),
            ImVec2(1, 0)
        );

        if (m_EditorCamera->ViewportNeedsResize(width, height))
        {
            Renderer::ResizeViewport(width, height);
            m_EditorCamera->OnResize(width, height);
            m_Framebuffer->ResizeFrameBuffer(width, height);
        }
    }
    ImGui::End();
    ImGui::PopStyleVar();
}

bool EditorLayer::OnKeyPressed(KeyPressedEvent& e)
{
    return true;
}

bool EditorLayer::OnMouseButtonPressed(MouseButtonPressedEvent& e)
{
    return true;
}
