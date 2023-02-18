#include "pch.h"
#include "Skybox.h"

Skybox::Skybox(const std::string& path)
    : m_Path(path)
{
    m_VertexBuffer = new VertexBuffer(c_SkyboxVertices, sizeof(c_SkyboxVertices));
    m_VertexBuffer->SetLayout({{ShaderDataType::Float3, "position"}});
    m_VertexArray = new VertexArray();
    m_VertexArray->AddVertexBuffer(m_VertexBuffer);

    m_Texture = new Texture2D(path, ETextureUsage::CubeMap);

    m_Shader = new Shader("Shaders/SkyBox");
}

void Skybox::Bind() const
{
    m_Shader->Bind();
    m_VertexArray->Bind();
    m_Texture->Bind();
}

void Skybox::UnBind() const
{
    m_Shader->UnBind();
    m_VertexArray->UnBind();
    m_Texture->UnBind();
}

void Skybox::LoadCubeMap(const std::string& path)
{
    m_Texture = new Texture2D(path, ETextureUsage::CubeMap);
    m_Path = path;
}
