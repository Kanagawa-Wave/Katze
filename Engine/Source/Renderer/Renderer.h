#pragma once
#include <glm/glm.hpp>

#include "Scene/Components.h"
#include "Scene/Scene.h"

class Renderer
{
public:
    static void Init();
    static void SetClearColor(const glm::vec4& color);
    static void Clear();
    
    static void ResizeViewport(unsigned width, unsigned height);
    
    static void Begin(const Camera& camera);
    static void End();

    static void DrawMesh(const TransformComponent& transform, const MeshComponent& mesh);

    struct Stats
    {
        uint32_t DrawCalls = 0;
        uint32_t VertexCount = 0;
    };

    struct RenderData
    {
        struct GlobalUbo
        {
            glm::mat4 projection{};
            glm::mat4 view{};
            glm::vec4 ambientColor{};
            glm::vec3 lightPosition{};
            alignas(16) glm::vec4 lightColor{}; // w is lightIntensity
        };

        Stats RenderStats;
        GlobalUbo UniformData;
        UniformBuffer* UniformBuffer = nullptr;
    };

    static void ResetStats();
    static Stats GetStats();

    static RenderData GetRenderData();
    static RenderData::GlobalUbo GetUniformData();
    static void SetRenderData(const RenderData& data);
    static void SetUniformData(const RenderData::GlobalUbo& data);
    static void SetDirectionalLightPosition(const glm::vec3& position);
    static void SetDirectionalLightColor(const glm::vec4& color);
    static void SetAmbientColor(const glm::vec4& color);
    

private:
    inline static void IncrementDrawCall();
    inline static void AddVertexCount(uint32_t count);
};


