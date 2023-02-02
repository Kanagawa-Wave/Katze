#include "pch.h"

#include "Texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include "Core/Core.h"

#include <glad/glad.h>

Texture2D::Texture2D(const std::string& path)
    : m_path(path)
{
    int width, height, channels;
    stbi_set_flip_vertically_on_load(1);
    stbi_uc* data = stbi_load(path.c_str(), &width, &height, &channels, 0);
		
    if(stbi_failure_reason())
    {
        LOG_ERROR("stb_image error: {0}", stbi_failure_reason())
        LOG_ERROR("Error when loading image")
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

Texture2D::Texture2D(uint32_t width, uint32_t height)
    : m_width(width), m_height(height)
{
    glGenTextures(1, &m_texture);
    glBindTexture(GL_TEXTURE_2D, m_texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

Texture2D::~Texture2D()
{
    glDeleteTextures(1, &m_texture);
}

void Texture2D::Bind(const uint32_t& slot) const
{
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_2D, m_texture);
}

void Texture2D::UnBind() const
{
    glBindTexture(GL_TEXTURE_2D, 0);
}
