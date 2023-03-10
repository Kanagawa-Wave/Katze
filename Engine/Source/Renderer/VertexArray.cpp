#include "pch.h"

#include "VertexArray.h"

#include <glad/glad.h>


static GLenum ShaderDataTypeToOpenGLBaseType(ShaderDataType type)
{
    switch (type)
    {
    case ShaderDataType::Float: return GL_FLOAT;
    case ShaderDataType::Float2: return GL_FLOAT;
    case ShaderDataType::Float3: return GL_FLOAT;
    case ShaderDataType::Float4: return GL_FLOAT;
    case ShaderDataType::Mat3: return GL_FLOAT;
    case ShaderDataType::Mat4: return GL_FLOAT;
    case ShaderDataType::Int: return GL_INT;
    case ShaderDataType::Int2: return GL_INT;
    case ShaderDataType::Int3: return GL_INT;
    case ShaderDataType::Int4: return GL_INT;
    case ShaderDataType::Bool: return GL_BOOL;
    case ShaderDataType::None: LOG_ERROR("Unknown ShaderDataType")
    }

    return 0;
}

VertexArray::VertexArray()
{
    glCreateVertexArrays(1, &m_vao);
}

VertexArray::~VertexArray()
{
    glDeleteVertexArrays(1, &m_vao);
}

void VertexArray::Bind() const
{
    glBindVertexArray(m_vao);
}

void VertexArray::UnBind() const
{
    glBindVertexArray(0);
}

void VertexArray::AddVertexBuffer(VertexBuffer* vertexBuffer)
{
    if (vertexBuffer->GetLayout().GetElements().empty())
        LOG_ERROR("Vertex Buffer missing a Layout")

    uint32_t index = 0;
    BufferLayout layout = vertexBuffer->GetLayout();
    for (const auto& element : layout)
    {
        glEnableVertexArrayAttrib(m_vao, index);
        glVertexArrayAttribFormat(
            m_vao,
            index,
            (GLint)element.GetComponentCount(), // GLuint size
            ShaderDataTypeToOpenGLBaseType(element.Type), // GLenum type
            element.Normalized ? GL_TRUE : GL_FALSE, // GLboolean normalized
            0 // const void* offset
        );
        glVertexArrayAttribBinding(m_vao, index, index);
        glVertexArrayVertexBuffer(m_vao, index, vertexBuffer->GetID(), element.Offset, (GLint)layout.GetStride());
        
        index++;
    }
    m_vertex_buffers.push_back(vertexBuffer);
}

void VertexArray::SetIndexBuffer(IndexBuffer* indexBuffer)
{
    glVertexArrayElementBuffer(m_vao, indexBuffer->GetID());

    m_index_buffer = indexBuffer;
}
