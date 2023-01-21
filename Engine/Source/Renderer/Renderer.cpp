#include "pch.h"

#include "Renderer.h"

#include <ranges>
#include <glad/glad.h>

#include "Core/Application.h"
#include "Core/Application.h"

void Renderer::Init()
{
    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
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

void Renderer::Submit(Scene& scene)
{
    for (const auto& [id, object] : scene.Objects)
    {
        object.GetMesh().GetShader()->Bind();
        object.GetMesh().GetVertexArray()->Bind();
        object.GetMesh().GetTexture()->Bind();

        glm::mat4 model, view, projection, normal;
        model = object.GetTransform().GetModelMat();
        view = scene.camera->GetView();
        projection = scene.camera->GetProjection();
        normal = object.GetTransform().GetNormalMat();
        
        scene.uboData.projection = projection;
        scene.uboData.view = view;
        scene.SetData();

        object.GetMesh().GetShader()->UploadUniformMat4("model", model);
        object.GetMesh().GetShader()->UploadUniformMat4("normal", normal);
        object.GetMesh().GetShader()->UploadUniformBuffer("GlobalUbo", scene.ubo);

        glDrawElements(GL_TRIANGLES, (GLsizei)object.GetMesh().GetVertexArray()->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);

        object.GetMesh().GetShader()->UnBind();
        object.GetMesh().GetVertexArray()->UnBind();
        object.GetMesh().GetTexture()->UnBind();
    }
}
