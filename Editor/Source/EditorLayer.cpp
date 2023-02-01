#include "EditorLayer.h"

#include "Outliner/Outliner.h"

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
    m_Scene = new Scene();
    m_Outliner = new Outliner(m_Scene);

    Entity smoothVase = m_Scene->CreateStaticMesh("smoothVase", "Assets/smooth_vase.obj");
    smoothVase.GetComponent<TransformComponent>().Rotation.z = glm::pi<float>();
    smoothVase.GetComponent<TransformComponent>().Scale = {3.f, 3.f, 3.f};
    smoothVase.GetComponent<TransformComponent>().Translation = {0.5f, -0.5f, 0.f};
    
    Entity flatVase = m_Scene->CreateStaticMesh("flatVase", "Assets/flat_vase.obj");
    flatVase.GetComponent<TransformComponent>().Rotation.z = glm::pi<float>();
    flatVase.GetComponent<TransformComponent>().Scale = {3.f, 3.f, 3.f};
    flatVase.GetComponent<TransformComponent>().Translation = {-0.5f, -0.5f, 0.f};

    Entity floor = m_Scene->CreateStaticMesh("floor", "Assets/floor.obj");
    floor.GetComponent<TransformComponent>().Rotation.z = glm::pi<float>();
    floor.GetComponent<TransformComponent>().Scale = {3.f, 1.f, 3.f};
    floor.GetComponent<TransformComponent>().Translation = {0.f, -0.5f, 0.f};
}

void EditorLayer::OnDetach()
{
}

void EditorLayer::OnEvent(Event& event)
{
    m_EditorCamera->OnEvent(event);
}

void EditorLayer::OnUpdate(Timestep& ts)
{
    m_TS = ts;
    m_EditorCamera->OnUpdate(ts);

    Renderer::SetClearColor({0.0f, 0.2f, 0.2f, 0.8f});
    Renderer::Clear();

    m_Framebuffer->Bind();
    
    m_Scene->RenderScene(*m_EditorCamera);

    m_Framebuffer->UnBind();
}

void EditorLayer::OnImGuiRender()
{
    DockSpace();
    EditorPanels();
    Viewport();
    StatsOverlay();
    
    m_Outliner->OnImGuiRender();
}

void EditorLayer::DockSpace()
{
    static bool opt_fullscreen = true;
    static bool opt_padding = false;
    static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

    // We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
    // because it would be confusing to have two docking targets within each others.
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
    if (opt_fullscreen)
    {
        const ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->WorkPos);
        ImGui::SetNextWindowSize(viewport->WorkSize);
        ImGui::SetNextWindowViewport(viewport->ID);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
        window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize |
            ImGuiWindowFlags_NoMove;
        window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
    }
    else
    {
        dockspace_flags &= ~ImGuiDockNodeFlags_PassthruCentralNode;
    }

    // When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background
    // and handle the pass-thru hole, so we ask Begin() to not render a background.
    if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
        window_flags |= ImGuiWindowFlags_NoBackground;

    // Important: note that we proceed even if Begin() returns false (aka window is collapsed).
    // This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
    // all active windows docked into it will lose their parent and become undocked.
    // We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
    // any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
    if (!opt_padding)
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    ImGui::Begin("DockSpace Demo", nullptr, window_flags);
    if (!opt_padding)
        ImGui::PopStyleVar();

    if (opt_fullscreen)
        ImGui::PopStyleVar(2);

    // Submit the DockSpace
    ImGuiIO& io = ImGui::GetIO();
    if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
    {
        ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
        ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
    }

    if (ImGui::BeginMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            ImGui::MenuItem("New GameObject");
            ImGui::EndMenu();
        }
        ImGui::EndMenuBar();
    }

    ImGui::End();
}

void EditorLayer::Viewport()
{
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

void EditorLayer::EditorPanels()
{
    glm::vec3 lightPos = Renderer::GetUniformData().lightPosition;
    glm::vec4 lightColor = Renderer::GetUniformData().lightColor;
    glm::vec4 ambientColor = Renderer::GetUniformData().ambientColor;
    
    ImGui::Begin("Config");
    ImGui::Text("Point Light");
    ImGui::SliderFloat3("Light Position", glm::value_ptr(lightPos), -5.f, 5.f);
    ImGui::ColorEdit3("Light Color", glm::value_ptr(lightColor));
    ImGui::SliderFloat("Light Intensity", &lightColor.w, 0.f, 1.f);
    ImGui::ColorEdit3("Ambient Color", glm::value_ptr(ambientColor));
    ImGui::End();

    Renderer::SetDirectionalLightPosition(lightPos);
    Renderer::SetDirectionalLightColor(lightColor);
    Renderer::SetAmbientColor(ambientColor);
}

void EditorLayer::StatsOverlay()
{
    static int location = 3;
    ImGuiIO& io = ImGui::GetIO();
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoDocking |
        ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing |
        ImGuiWindowFlags_NoNav;
    if (location >= 0)
    {
        const float PAD = 10.0f;
        const ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImVec2 work_pos = viewport->WorkPos; // Use work area to avoid menu-bar/task-bar, if any!
        ImVec2 work_size = viewport->WorkSize;
        ImVec2 window_pos, window_pos_pivot;
        window_pos.x = (location & 1) ? (work_pos.x + work_size.x - PAD) : (work_pos.x + PAD);
        window_pos.y = (location & 2) ? (work_pos.y + work_size.y - PAD) : (work_pos.y + PAD);
        window_pos_pivot.x = (location & 1) ? 1.0f : 0.0f;
        window_pos_pivot.y = (location & 2) ? 1.0f : 0.0f;
        ImGui::SetNextWindowPos(window_pos, ImGuiCond_Always, window_pos_pivot);
        ImGui::SetNextWindowViewport(viewport->ID);
        window_flags |= ImGuiWindowFlags_NoMove;
    }
    else if (location == -2)
    {
        // Center window
        ImGui::SetNextWindowPos(ImGui::GetMainViewport()->GetCenter(), ImGuiCond_Always, ImVec2(0.5f, 0.5f));
        window_flags |= ImGuiWindowFlags_NoMove;
    }
    ImGui::SetNextWindowBgAlpha(0.35f); // Transparent background
    if (ImGui::Begin("Overlay", nullptr, window_flags))
    {
        ImGui::Text("Frame Stats");
        ImGui::Separator();
        ImGui::Text("Frametime: %.1f ms", (double)m_TS.GetMilliseconds());
        ImGui::Text("FPS: %d", (int)(1.0 / (double)m_TS.GetSeconds()));
        ImGui::Separator();
        ImGui::Text("Render Stats");
        ImGui::Separator();
        ImGui::Text("Draw Calls: %d", Renderer::GetStats().DrawCalls);
        ImGui::Text("Vertex Count: %d", Renderer::GetStats().VertexCount);
        ImGui::Separator();
        if (ImGui::IsMousePosValid())
            ImGui::Text("Mouse Position: (%.1f,%.1f)", io.MousePos.x, io.MousePos.y);
        else
            ImGui::Text("Mouse Position: <invalid>");
    }
    ImGui::End();

    Renderer::ResetStats();
}

bool EditorLayer::OnKeyPressed(KeyPressedEvent& e)
{
    return true;
}

bool EditorLayer::OnMouseButtonPressed(MouseButtonPressedEvent& e)
{
    return true;
}
