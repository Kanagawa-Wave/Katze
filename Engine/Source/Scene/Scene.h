#pragma once

#include "Camera.h"

#include <entt.hpp>

class Entity;

class Scene
{
public:
    using Map = std::unordered_map<uint32_t, entt::entity>;
    
    Scene();
    ~Scene();
    
    Entity CreateEntity(const std::string& name = std::string());
    Entity CreateStaticMesh(const std::string& name = std::string(), const std::string& meshPath = std::string(), const std::string& texturePath = "Assets/white.png");
    Entity CreateEntityWithUUID(uint32_t uuid, const std::string& name = std::string());
    void DestroyEntity(Entity entity);

    Entity FindEntityByName(std::string_view name);
    Entity GetEntityByUUID(uint32_t uuid);

    bool IsRunning() const { return m_IsRunning; }
    bool IsPaused() const { return m_IsPaused; }

    void SetPaused(bool paused) { m_IsPaused = paused; }

    void Step(int frames = 1);

    template<typename... Components>
    auto GetAllEntitiesWith()
    {
        return m_Registry.view<Components...>();
    }
    void RenderScene(const Camera& camera);
private:

private:
    entt::registry m_Registry;
    uint32_t m_ViewportWidth = 0, m_ViewportHeight = 0;
    bool m_IsRunning = false;
    bool m_IsPaused = false;
    int m_StepFrames = 0;

    Map m_EntityMap;

    friend class Entity;
    friend class Outliner;
};
