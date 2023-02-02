#include "pch.h"

#include "Renderer.h"

#include <glad/glad.h>

static Renderer::RenderData s_Data;

void Renderer::Init()
{
    s_Data.UniformBuffer = new UniformBuffer(sizeof(RenderData::GlobalUbo), 0);
    s_Data.UniformData = RenderData::GlobalUbo();
    s_Data.UniformData.lightColor = {1.f, 1.f, 1.f, 1.f};
    s_Data.UniformData.ambientColor = {0.f, 0.f, 0.f, 1.f};

    s_Data.BillboardShader = new Shader("Shaders/Billboard");

    float OFFSETS[] = {
        -1.0, -1.0,
        -1.0, 1.0,
        1.0, -1.0,
        1.0, -1.0,
        -1.0, 1.0,
        1.0, 1.0
    };
    s_Data.BillboardVB = new VertexBuffer(OFFSETS, sizeof(OFFSETS));
    s_Data.BillboardVB->SetLayout({{ShaderDataType::Float2, "offset"}});

    s_Data.BillboardVA = new VertexArray();
    s_Data.BillboardVA->AddVertexBuffer(s_Data.BillboardVB);

    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);
}

void Renderer::SetClearColor(const glm::vec4& color)
{
    glClearColor(color.r, color.g, color.b, color.a);
}

void Renderer::Clear()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::ResizeViewport(unsigned width, unsigned height)
{
    glViewport(0, 0, (GLsizei)width, (GLsizei)height);
}

void Renderer::Begin(const Camera& camera)
{
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    s_Data.UniformData.projection = camera.GetProjection();
    s_Data.UniformData.view = camera.GetView();

    s_Data.UniformBuffer->SetData(&s_Data.UniformData, sizeof(RenderData::UniformData));
}

void Renderer::End()
{
    glDisable(GL_CULL_FACE);
    
    s_Data.BillboardShader->Bind();
    s_Data.BillboardVA->Bind();
    
    glDrawArrays(GL_TRIANGLES, 0, 6);
    AddVertexCount(6);
    IncrementDrawCall();
    
    s_Data.BillboardVA->UnBind();
    s_Data.BillboardShader->UnBind();
}

void Renderer::DrawMesh(const TransformComponent& transform, const MeshComponent& mesh)
{
    auto& staticMesh = mesh.StaticMesh;
    staticMesh.GetShader()->Bind();
    staticMesh.GetVertexArray()->Bind();
    staticMesh.GetTexture()->Bind();

    staticMesh.GetShader()->UploadUniformMat4("model", transform.GetModelMat());
    staticMesh.GetShader()->UploadUniformMat4("normal", transform.GetNormalMat());

    const uint32_t vertexCount = (GLsizei)staticMesh.GetVertexArray()->GetIndexBuffer()->GetCount();
    glDrawElements(GL_TRIANGLES, (GLsizei)vertexCount, GL_UNSIGNED_INT, nullptr);
    AddVertexCount(vertexCount);
    IncrementDrawCall();

    staticMesh.GetShader()->UnBind();
    staticMesh.GetVertexArray()->UnBind();
    staticMesh.GetTexture()->UnBind();
}

void Renderer::ResetStats()
{
    memset(&s_Data.RenderStats, 0, sizeof(Stats));
}

Renderer::Stats Renderer::GetStats()
{
    return s_Data.RenderStats;
}

Renderer::RenderData Renderer::GetRenderData()
{
    return s_Data;
}

Renderer::RenderData::GlobalUbo Renderer::GetUniformData()
{
    return s_Data.UniformData;
}

void Renderer::SetRenderData(const RenderData& data)
{
    s_Data = data;
}

void Renderer::SetUniformData(const RenderData::GlobalUbo& data)
{
    s_Data.UniformData = data;
}

void Renderer::SetDirectionalLightPosition(const glm::vec3& position)
{
    s_Data.UniformData.lightPosition = position;
}

void Renderer::SetDirectionalLightColor(const glm::vec4& color)
{
    s_Data.UniformData.lightColor = color;
}

void Renderer::SetAmbientColor(const glm::vec4& color)
{
    s_Data.UniformData.ambientColor = color;
}

inline void Renderer::IncrementDrawCall()
{
    s_Data.RenderStats.DrawCalls++;
}

inline void Renderer::AddVertexCount(uint32_t count)
{
    s_Data.RenderStats.VertexCount += count;
}
