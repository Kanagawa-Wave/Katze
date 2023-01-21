#pragma once

typedef unsigned int GLenum;

class Texture2D
{
public:
    Texture2D() = default;
    Texture2D(const std::string& path);
    Texture2D(uint32_t width, uint32_t height);
    ~Texture2D();

    uint32_t GetWidth() const { return m_width; }
    uint32_t GetHeight() const { return m_height; }
    uint32_t GetTexture() const { return m_texture; }

    void Bind(const uint32_t& slot = 0) const;
    void UnBind() const;
private:
    std::string m_path;
    bool m_isLoaded = false;
    uint32_t m_width, m_height, m_texture;
    GLenum m_internalFormat, m_dataFormat;
};
