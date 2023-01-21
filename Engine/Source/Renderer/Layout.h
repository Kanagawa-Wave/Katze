﻿#pragma once

#include "Logger/Log.h"

enum class ShaderDataType : uint8_t
{
    None = 0,
    Float, Float2, Float3, Float4,
    Mat3, Mat4,
    Int, Int2, Int3, Int4,
    Bool
};

static uint32_t GetShaderDataTypeSize(ShaderDataType type)
{
    switch (type)
    {
    case ShaderDataType::Float:     return 4;
    case ShaderDataType::Float2:    return 4 * 2;
    case ShaderDataType::Float3:    return 4 * 3;
    case ShaderDataType::Float4:    return 4 * 4;
    case ShaderDataType::Mat3:      return 4 * 3 * 3;
    case ShaderDataType::Mat4:      return 4 * 4 * 4;
    case ShaderDataType::Int:       return 4;
    case ShaderDataType::Int2:      return 4 * 2;
    case ShaderDataType::Int3:      return 4 * 3;
    case ShaderDataType::Int4:      return 4 * 4;
    case ShaderDataType::Bool:      return 1;
    case ShaderDataType::None:
        LOG_ERROR("ShaderDataType = None!")
        return 0;
    default: 
        LOG_ERROR("Invalid ShaderDataType!")
        return 0;
    }
}

struct BufferElement
{
    std::string Name;
    ShaderDataType Type;
    uint32_t Size;
    uint32_t Offset;
    bool Normalized;

    uint32_t GetComponentCount() const
    {
        switch (Type)
        {
        case ShaderDataType::Float:   return 1;
        case ShaderDataType::Float2:  return 2;
        case ShaderDataType::Float3:  return 3;
        case ShaderDataType::Float4:  return 4;
        case ShaderDataType::Mat3:    return 3 * 3;
        case ShaderDataType::Mat4:    return 4 * 4;
        case ShaderDataType::Int:     return 1;
        case ShaderDataType::Int2:    return 2;
        case ShaderDataType::Int3:    return 3;
        case ShaderDataType::Int4:    return 4;
        case ShaderDataType::Bool:    return 1;
        case ShaderDataType::None:    LOG_ERROR("Invalid ShaderDataType!")
        }
        
        return 0;
    }
	
    BufferElement() = default;

    BufferElement(ShaderDataType type, const std::string& name, bool normalized = false)
        : Name(name), Type(type), Size(GetShaderDataTypeSize(type)), Offset(0), Normalized(normalized) {}
};

class BufferLayout
{
public:
    BufferLayout() = default;
		
    BufferLayout(const std::initializer_list<BufferElement>& elements) : m_elements(elements)
    {
        uint32_t offset = 0;
        m_stride = 0;
        for (auto& element : m_elements)
        {
            element.Offset = offset;
            offset += element.Size;
            m_stride += element.Size;
        }
    }

    inline uint32_t GetStride() const { return m_stride; }
    inline const std::vector<BufferElement>& GetElements() const { return m_elements; }

    std::vector<BufferElement>::iterator begin() { return m_elements.begin(); }
    std::vector<BufferElement>::iterator end() { return m_elements.end(); }
    std::vector<BufferElement>::const_iterator begin() const { return m_elements.begin(); }
    std::vector<BufferElement>::const_iterator end() const { return m_elements.end(); }
    
private:
    std::vector<BufferElement> m_elements;
    uint32_t m_stride;
};