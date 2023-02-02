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

    uint32_t GetID() const;
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

class UniformBuffer
{
public:
    UniformBuffer(uint32_t size, uint32_t binding);
    ~UniformBuffer();

    void SetData(const void* data, uint32_t size, uint32_t offset = 0) const;
private:
    uint32_t m_ubo = 0;
};


