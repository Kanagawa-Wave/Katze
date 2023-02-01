#include "Outliner.h"

#include <imgui/imgui.h>

Outliner::Outliner(Scene* scene)
{
    SetContext(scene);
}

void Outliner::SetContext(Scene* scene)
{
    m_Context = scene;
}

void Outliner::OnImGuiRender()
{
    ImGui::Begin("Outliner");
    m_Context->m_Registry.each([&](auto enttHandle)
    {
        Entity entity(enttHandle, m_Context);
        DrawEntityNode(entity);
    });
    ImGui::End();
}

void Outliner::DrawEntityNode(Entity entity)
{
    ImGuiTreeNodeFlags flags = ((m_SelectedEntity == entity) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
    
    std::string tag = entity.GetComponent<TagComponent>().Tag;
    bool opened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)entity, flags, tag.c_str());

    if (ImGui::IsItemClicked())
    {
        m_SelectedEntity = entity;
    }

    if (opened)
    {
        ImGuiTreeNodeFlags flagsChild = ImGuiTreeNodeFlags_OpenOnArrow;
        bool openedChild = ImGui::TreeNodeEx((void*)9817239, flagsChild, tag.c_str());
        if (openedChild)
            ImGui::TreePop();
        ImGui::TreePop();
    }
}
