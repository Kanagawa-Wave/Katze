#pragma once

#include "Camera.h"
#include "Renderer/Shader.h"
#include "Renderer/Buffers.h"
#include "Renderer/Texture.h"
#include "Renderer/VertexArray.h"
#include "Components.h"

class MeshComponent
{
public:
    struct Vertex
    {
        glm::vec3 position{};
        glm::vec3 color{};
        glm::vec3 normal{};
        glm::vec2 uv{};

        bool operator ==(const Vertex& other) const
        {
            return
                position == other.position &&
                color == other.color &&
                normal == other.normal &&
                uv == other.uv;
        }

        static BufferLayout Layout()
        {
            return {
                    {ShaderDataType::Float3, "position"},
                    {ShaderDataType::Float3, "color"},
                    {ShaderDataType::Float3, "normal"},
                    {ShaderDataType::Float2, "uv"}
            };
        }
    };
    
    MeshComponent() = default;
    MeshComponent(const std::string& path);
    MeshComponent(const std::string& meshPath, const std::string& shaderPath, const std::string& texturePath);

    void LoadMeshFromFile(const std::string& path);
    void SetShader(const std::string& path);
    void SetTexture(const std::string& path);
    void SetTexture();
    
    inline const Shader* GetShader() const { return m_shader; }
    inline const VertexArray* GetVertexArray() const { return m_vertexArray; }
    inline const Texture2D* GetTexture() const { return m_texture; }

    
private:
    std::vector<Vertex> m_vertices{};
    std::vector<uint32_t> m_indices{};

    Shader* m_shader = nullptr;
    Texture2D* m_texture = nullptr;
    VertexArray* m_vertexArray = nullptr;
    VertexBuffer* m_vertexBuffer = nullptr;
    IndexBuffer* m_indexBuffer = nullptr;
};
