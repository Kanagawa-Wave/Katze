#pragma once
#include "Renderer/Shader.h"
#include "Renderer/Texture.h"
#include "Renderer/VertexArray.h"

class Skybox
{
public:
    Skybox() = default;
    Skybox(const std::string& path);

    void Bind() const;
    void UnBind() const;

    void LoadCubeMap(const std::string& path);
    std::string GetPath()
    {
        m_Path.resize(256);
        return m_Path;
    }
    Shader* GetShader() const { return m_Shader; }

private:
    std::string m_Path{};
    Shader* m_Shader = nullptr;
    Texture2D* m_Texture = nullptr;
    VertexArray* m_VertexArray = nullptr;
    VertexBuffer* m_VertexBuffer = nullptr;

    const float c_SkyboxVertices[36*3] = {
        // Positions          
        -1000.0f, 1000.0f, -1000.0f,
        -1000.0f, -1000.0f, -1000.0f,
        1000.0f, -1000.0f, -1000.0f,
        1000.0f, -1000.0f, -1000.0f,
        1000.0f, 1000.0f, -1000.0f,
        -1000.0f, 1000.0f, -1000.0f,

        -1000.0f, -1000.0f, 1000.0f,
        -1000.0f, -1000.0f, -1000.0f,
        -1000.0f, 1000.0f, -1000.0f,
        -1000.0f, 1000.0f, -1000.0f,
        -1000.0f, 1000.0f, 1000.0f,
        -1000.0f, -1000.0f, 1000.0f,

        1000.0f, -1000.0f, -1000.0f,
        1000.0f, -1000.0f, 1000.0f,
        1000.0f, 1000.0f, 1000.0f,
        1000.0f, 1000.0f, 1000.0f,
        1000.0f, 1000.0f, -1000.0f,
        1000.0f, -1000.0f, -1000.0f,

        -1000.0f, -1000.0f, 1000.0f,
        -1000.0f, 1000.0f, 1000.0f,
        1000.0f, 1000.0f, 1000.0f,
        1000.0f, 1000.0f, 1000.0f,
        1000.0f, -1000.0f, 1000.0f,
        -1000.0f, -1000.0f, 1000.0f,

        -1000.0f, 1000.0f, -1000.0f,
        1000.0f, 1000.0f, -1000.0f,
        1000.0f, 1000.0f, 1000.0f,
        1000.0f, 1000.0f, 1000.0f,
        -1000.0f, 1000.0f, 1000.0f,
        -1000.0f, 1000.0f, -1000.0f,

        -1000.0f, -1000.0f, -1000.0f,
        -1000.0f, -1000.0f, 1000.0f,
        1000.0f, -1000.0f, -1000.0f,
        1000.0f, -1000.0f, -1000.0f,
        -1000.0f, -1000.0f, 1000.0f,
        1000.0f, -1000.0f, 1000.0f
    };
};
