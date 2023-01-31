#include "pch.h"

#include "Buffers.h"

#include <glad/glad.h>

VertexBuffer::VertexBuffer(float* vertices, uint32_t size)
{
    // glGenBuffers(1, &m_vbo);
    // glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    // glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
    glCreateBuffers(1, &m_vbo);
    glNamedBufferStorage(m_vbo, size, vertices, GL_DYNAMIC_STORAGE_BIT);
}

VertexBuffer::VertexBuffer(void* data, uint32_t size)
{
    // glGenBuffers(1, &m_vbo);
    // glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    // glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
    glCreateBuffers(1, &m_vbo);
    glNamedBufferStorage(m_vbo, size, data, GL_DYNAMIC_STORAGE_BIT);
}

VertexBuffer::~VertexBuffer()
{
    glDeleteBuffers(1, &m_vbo);
}

void VertexBuffer::Bind() const
{
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
}

void VertexBuffer::UnBind() const
{
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

IndexBuffer::IndexBuffer(uint32_t* indices, uint32_t count)
    : m_count(count)
{
    glGenBuffers(1, &m_ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32_t), indices, GL_STATIC_DRAW);
}

IndexBuffer::~IndexBuffer()
{
    glDeleteBuffers(1, &m_ibo);
}

void IndexBuffer::Bind() const
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);
}

void IndexBuffer::UnBind() const
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

uint32_t IndexBuffer::GetCount() const
{
    return m_count;
}

uint32_t IndexBuffer::GetID() const
{
    return m_ibo;
}

UniformBuffer::UniformBuffer(uint32_t size, uint32_t binding)
{
    glCreateBuffers(1, &m_ubo);
    glNamedBufferData(m_ubo, size, nullptr, GL_DYNAMIC_DRAW);
    glBindBufferBase(GL_UNIFORM_BUFFER, binding, m_ubo);
}

void UniformBuffer::SetData(const void* data, uint32_t size, uint32_t offset) const
{
    glNamedBufferSubData(m_ubo, offset, size, data);
}

UniformBuffer::~UniformBuffer()
{
    glDeleteBuffers(1, &m_ubo);
}
