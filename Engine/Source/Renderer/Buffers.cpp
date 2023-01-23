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

UniformBuffer::~UniformBuffer()
{
    glDeleteBuffers(1, &m_ubo);
}

void UniformBuffer::Init()
{
    glGenBuffers(1, &m_ubo);
    glBindBuffer(GL_UNIFORM_BUFFER, m_ubo);
    glBufferData(GL_UNIFORM_BUFFER, sizeof(GlobalUbo), nullptr, GL_STATIC_DRAW);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void UniformBuffer::Bind() const
{
    glBindBufferBase(GL_UNIFORM_BUFFER, 0, m_ubo);
    glBindBuffer(GL_UNIFORM_BUFFER, m_ubo);
}

void UniformBuffer::UnBind() const
{
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

uint32_t UniformBuffer::GetID() const
{
    return m_ubo;
}


uint32_t UniformBuffer::GetIndex() const
{
    return m_index;
}


void UniformBuffer::SetData(const GlobalUbo& data)
{
    m_data = data;
}


GlobalUbo& UniformBuffer::GetData()
{
    return m_data;
}
