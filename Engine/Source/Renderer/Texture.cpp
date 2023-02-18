#include "pch.h"

#include "Texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include "Core/Core.h"

#include <glad/glad.h>

Texture2D::Texture2D(const std::string& path, ETextureUsage usage)
    : m_path(path), m_Usage(usage)
{
    switch (usage)
    {
    case ETextureUsage::Texture:
        LoadTexture();
        break;
    case ETextureUsage::ImageTexture:
        CORE_ASSERT(0, "Image Texture can only be used as compute shader output!")
        break;
    case ETextureUsage::CubeMap:
        LoadCubeMap();
        break;
    }
}

Texture2D::Texture2D(uint32_t width, uint32_t height, ETextureUsage usage)
    : m_width(width), m_height(height), m_Usage(usage)
{
    switch (usage)
    {
    case ETextureUsage::ImageTexture:
        CreateComputeTexture(width, height);
        break;
    case ETextureUsage::FramebufferTexture:
        CreateFramebufferTexture(width, height);
        break;
    }
}

Texture2D::~Texture2D()
{
    glDeleteTextures(1, &m_texture);
}

void Texture2D::Bind(const uint32_t& slot) const
{
    glActiveTexture(GL_TEXTURE0 + slot);
    if (m_Usage == ETextureUsage::CubeMap)
        glBindTexture(GL_TEXTURE_CUBE_MAP, m_texture);
    else
        glBindTexture(GL_TEXTURE_2D, m_texture);
}

void Texture2D::UnBind() const
{
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture2D::LoadTexture()
{
    int width, height, channels;
    stbi_set_flip_vertically_on_load(1);
    stbi_uc* data = stbi_load(m_path.c_str(), &width, &height, &channels, 0);

    if (stbi_failure_reason())
    {
        if (stbi_failure_reason() != "bad png sig")
        {
            LOG_ERROR("stb_image error: {0}", stbi_failure_reason())
            LOG_ERROR("Error when loading image")
        }
    }

    if (data)
    {
        m_isLoaded = true;

        m_width = width;
        m_height = height;

        GLenum internalFormat = 0, dataFormat = 0;
        if (channels == 4)
        {
            internalFormat = GL_RGBA8;
            dataFormat = GL_RGBA;
        }
        else if (channels == 3)
        {
            internalFormat = GL_RGB8;
            dataFormat = GL_RGB;
        }

        m_internalFormat = internalFormat;
        m_dataFormat = dataFormat;

        if (!(internalFormat & dataFormat))
            LOG_ERROR("Format not supported!")

        glCreateTextures(GL_TEXTURE_2D, 1, &m_texture);

        glTextureStorage2D(m_texture, 4, internalFormat, width, height);

        glTextureParameteri(m_texture, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTextureParameteri(m_texture, GL_TEXTURE_WRAP_T, GL_REPEAT);

        glTextureParameteri(m_texture, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTextureParameteri(m_texture, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glTextureSubImage2D(m_texture, 0, 0, 0, width, height, dataFormat, GL_UNSIGNED_BYTE, data);
        glGenerateTextureMipmap(m_texture);

        stbi_image_free(data);
    }
}


// Loads a cubemap texture from 6 individual texture faces
// Order should be:
// +X (right)
// -X (left)
// +Y (top)
// -Y (bottom)
// +Z (front)
// -Z (back)
void Texture2D::LoadCubeMap()
{
    std::vector<std::string> faces;
    faces.push_back(m_path + "/posx.jpg");
    faces.push_back(m_path + "/negx.jpg");
    faces.push_back(m_path + "/posy.jpg");
    faces.push_back(m_path + "/negy.jpg");
    faces.push_back(m_path + "/posz.jpg");
    faces.push_back(m_path + "/negz.jpg");

    int width, height, channels;
    stbi_uc* data = nullptr;

    stbi_set_flip_vertically_on_load(0);
    
    glCreateTextures(GL_TEXTURE_CUBE_MAP, 1, &m_texture);
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_texture);

    for (GLuint i = 0; i < faces.size(); i++)
    {
        data = stbi_load(faces[i].c_str(), &width, &height, &channels, 0);
        
        if (stbi_failure_reason())
        {
            if (stbi_failure_reason() != "bad png sig")
            {
                LOG_ERROR("stb_image error: {0}", stbi_failure_reason())
                LOG_ERROR("Error when loading image")
            }
        }

        if (data)
        {
            m_isLoaded = true;

            m_width = width;
            m_height = height;

            GLenum internalFormat = 0, dataFormat = 0;
            if (channels == 4)
            {
                internalFormat = GL_RGBA8;
                dataFormat = GL_RGBA;
            }
            else if (channels == 3)
            {
                internalFormat = GL_RGB8;
                dataFormat = GL_RGB;
            }

            m_internalFormat = internalFormat;
            m_dataFormat = dataFormat;
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, (GLint)internalFormat, width, height, 0, dataFormat,
                         GL_UNSIGNED_BYTE, data);
            stbi_image_free(data);
        }
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
}

void Texture2D::CreateFramebufferTexture(uint32_t width, uint32_t height)
{
    m_internalFormat = GL_RGBA8;
    m_dataFormat = GL_RGBA;

    glCreateTextures(GL_TEXTURE_2D, 1, &m_texture);
    glTextureStorage2D(m_texture, 0, m_internalFormat, (GLsizei)width, (GLsizei)height);

    glTextureParameteri(m_texture, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTextureParameteri(m_texture, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glTextureParameteri(m_texture, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTextureParameteri(m_texture, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTextureSubImage2D(m_texture, 0, 0, 0, (GLsizei)width, (GLsizei)height, m_dataFormat, GL_FLOAT, nullptr);
}

void Texture2D::CreateComputeTexture(uint32_t width, uint32_t height)
{
    glGenTextures(1, &m_texture);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, (GLsizei)width, (GLsizei)height, 0, GL_RGBA, GL_FLOAT, nullptr);
}
