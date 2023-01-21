#pragma once

#include "Buffers.h"

class VertexArray
{
public:
    VertexArray();
    ~VertexArray();

    void Bind() const;
    void UnBind() const;

    void AddVertexBuffer(VertexBuffer* vertexBuffer);
    void SetIndexBuffer(IndexBuffer* indexBuffer);

    const std::vector<VertexBuffer*>& GetVertexBuffers() const { return m_vertex_buffers; }
    const IndexBuffer* GetIndexBuffer() const { return m_index_buffer; }
private:
    std::vector<VertexBuffer*> m_vertex_buffers;
    IndexBuffer* m_index_buffer = nullptr;
		
    uint32_t m_vao = 0;
};
