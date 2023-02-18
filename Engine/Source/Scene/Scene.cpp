#include "pch.h"

#include "Scene.h"
#include "Entity.h"
#include "Renderer/Renderer.h"

Scene::Scene()
{
}

Scene::~Scene()
{
}

Entity Scene::CreateEntity(const std::string& name)
{
    return CreateEntityWithUUID(currentID++, name);
}

Entity Scene::CreateStaticMesh(const std::string& name, const std::string& meshPath, const std::string& texturePath)
{
    Entity staticMesh = CreateEntityWithUUID(currentID++, name);
    staticMesh.AddComponent<MeshComponent>(meshPath, texturePath);
    return staticMesh;
}

Entity Scene::CreateEntityWithUUID(uint32_t uuid, const std::string& name)
{
    Entity entity = Entity(m_Registry.create(), this);
    entity.AddComponent<IDComponent>(uuid);
    entity.AddComponent<TransformComponent>();
    auto& tag = entity.AddComponent<TagComponent>();
    tag.Tag = name.empty() ? "Entity" : name;

    m_EntityMap[uuid] = entity;

    return entity;
}

void Scene::DestroyEntity(Entity entity)
{
    m_EntityMap.erase(entity.GetUUID());
    m_Registry.destroy(entity);
}


Entity Scene::FindEntityByName(std::string_view name)
{
    const auto view = m_Registry.view<TagComponent>();
    for (auto entity : view)
    {
        const TagComponent& tc = view.get<TagComponent>(entity);
        if (tc.Tag == name)
            return {entity,this};
    }
    return {};
}

Entity Scene::GetEntityByUUID(uint32_t uuid)
{
    if (m_EntityMap.find(uuid) != m_EntityMap.end())
        return { m_EntityMap.at(uuid), this };

    return {};
}

void Scene::Step(int frames)
{
    m_StepFrames = frames;
}

void Scene::RenderScene(const Camera& camera)
{
    Renderer::Begin(camera);
    
    Renderer::SetClearColor({0.0f, 0.0f, 0.0f, 1.0f});
    Renderer::Clear();

    Renderer::DrawSkyBox(m_SkyBox);
    
    auto view = m_Registry.view<TransformComponent, MeshComponent>();
    for (auto entity : view)
    {
        auto [transform, mesh] = view.get<TransformComponent, MeshComponent>(entity);

        Renderer::DrawMesh(transform, mesh);
    }

    
    Renderer::End();
}

Entity Scene::SetSkyBox(const std::string& path)
{
    if (m_SkyBox)
        CORE_ASSERT(0, "Cannot have 2 skybox in 1 scene!")

    const uint32_t uuid = currentID++;
    Entity Skybox = Entity(m_Registry.create(), this);
    Skybox.AddComponent<IDComponent>(uuid);
    Skybox.AddComponent<TransformComponent>();
    auto& tag = Skybox.AddComponent<TagComponent>();
    tag.Tag = "Skybox";

    m_EntityMap[uuid] = Skybox;
    m_SkyBox = Skybox.AddComponent<SkyBoxComponent>(path).SkyBox;
    return Skybox;
}
