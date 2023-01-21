#pragma once

#include <glm/glm.hpp>

#include "Layout.h"

class VertexBuffer
{
public:
    VertexBuffer() = default;
    VertexBuffer(float* vertices, uint32_t size);
    VertexBuffer(void* data, uint32_t size);
    ~VertexBuffer();
    
    void Bind() const;
    void UnBind() const;

    const BufferLayout& GetLayout() const { return m_layout; }
    void SetLayout(const BufferLayout& layout) { m_layout = layout; }
private:
    uint32_t m_vbo = 0;
    BufferLayout m_layout{};
};

class IndexBuffer
{
public:
    IndexBuffer() = default;
    IndexBuffer(uint32_t* indices, uint32_t count);
    ~IndexBuffer();
    
    void Bind() const;
    void UnBind() const;

    uint32_t GetCount() const;
    uint32_t GetID() const;
private:
    uint32_t m_ibo = 0, m_count = 0;
};

struct GlobalUbo
{
    glm::mat4 projection;
    glm::mat4 view;
    glm::vec4 ambientColor;
    glm::vec3 lightPosition;
    alignas(16) glm::vec4 lightColor; // w is lightIntensity
};

class UniformBuffer
{
public:
    UniformBuffer() = default;
    ~UniformBuffer();

    void Init();
    
    void Bind() const;
    void UnBind() const;
    
    uint32_t GetID() const;
    uint32_t GetIndex() const;

    void SetData(const GlobalUbo& data);
    GlobalUbo& GetData();
private:
    uint32_t m_ubo = 0, m_index = 0;
    GlobalUbo m_data;
};


