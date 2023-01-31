#include "pch.h"

#include "Mesh.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/hash.hpp>
#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>

#include "Renderer/Renderer.h"
#include "Timer.h"

template <typename T, typename... Rest>
void hashCombine(std::size_t& seed, const T& v, const Rest&... rest)
{
    seed ^= std::hash<T>{}(v) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
    (hashCombine(seed, rest), ...);
};

namespace std
{
    template <>
    struct hash<Mesh::Vertex>
    {
        size_t operator()(Mesh::Vertex const& vertex) const noexcept
        {
            size_t seed = 0;
            hashCombine(seed, vertex.position, vertex.color, vertex.normal, vertex.uv);
            return seed;
        }
    };
}


Mesh::Mesh(const std::string& path)
{
    LOG_INFO("Loading mesh: {0}", path)
    Timer timer;
    LoadMeshFromFile(path);
    LOG_INFO("Mesh load complete: {0}s", timer.Elapsed())
}

Mesh::Mesh(const std::string& meshPath, const std::string& shaderPath, const std::string& texturePath)
{
    LOG_INFO("Loading mesh: {0}", meshPath)
    Timer timer;
    LoadMeshFromFile(meshPath);
    LOG_INFO("Mesh load complete: {0}s", timer.Elapsed())
    SetShader(shaderPath);
    SetTexture(texturePath);
}

void Mesh::LoadMeshFromFile(const std::string& path)
{
    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    std::string warn, error;

    if (!tinyobj::LoadObj(
        &attrib,
        &shapes,
        &materials,
        &warn,
        &error,
        path.c_str()))
    {
        LOG_ERROR("{0}", warn + error)
    }

    std::unordered_map<Vertex, uint32_t> uniqueVertices{};

    for (const auto& shape : shapes)
    {
        for (const auto& index : shape.mesh.indices)
        {
            Vertex vertex{};
            if (index.vertex_index >= 0)
            {
                vertex.position = {
                    attrib.vertices[3 * index.vertex_index + 0],
                    attrib.vertices[3 * index.vertex_index + 1],
                    attrib.vertices[3 * index.vertex_index + 2],
                };
                vertex.color = {
                    attrib.colors[3 * index.vertex_index + 0],
                    attrib.colors[3 * index.vertex_index + 1],
                    attrib.colors[3 * index.vertex_index + 2],
                };
            }
            if (index.normal_index >= 0)
            {
                vertex.normal = {
                    attrib.normals[3 * index.normal_index + 0],
                    attrib.normals[3 * index.normal_index + 1],
                    attrib.normals[3 * index.normal_index + 2],
                };
            }
            if (index.texcoord_index >= 0)
            {
                vertex.uv = {
                    attrib.texcoords[2 * index.texcoord_index + 0],
                    attrib.texcoords[2 * index.texcoord_index + 1],
                };
            }


            if (uniqueVertices.count(vertex) == 0)
            {
                uniqueVertices[vertex] = static_cast<uint32_t>(m_vertices.size());
                m_vertices.push_back(vertex);
            }
            m_indices.push_back(uniqueVertices[vertex]);
        }
    }
}

void Mesh::SetShader(const std::string& path)
{
    m_shader = new Shader(path);
    m_shader->Bind();

    m_vertexBuffer = new VertexBuffer(m_vertices.data(), (uint32_t)m_vertices.size() * sizeof(Vertex));
    m_vertexBuffer->SetLayout(Vertex::Layout());
    m_indexBuffer = new IndexBuffer(m_indices.data(), (uint32_t)m_indices.size());

    m_vertexArray = new VertexArray();
    m_vertexArray->Bind();
    m_vertexArray->AddVertexBuffer(m_vertexBuffer);
    m_vertexArray->SetIndexBuffer(m_indexBuffer);

    m_vertices.clear();
    m_indices.clear();

    m_vertexArray->UnBind();
    m_shader->UnBind();
}

void Mesh::SetTexture(const std::string& path)
{
    m_texture = new Texture2D(path);
    m_shader->UploadUniformInt("tex", 0);
}

void Mesh::SetTexture()
{
    m_texture = new Texture2D("Assets/white.png");
    m_shader->UploadUniformInt("tex", 0);
}
