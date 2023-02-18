#pragma once

typedef unsigned int GLenum;

enum class ETextureUsage
{
    Texture,
    FramebufferTexture,
    ImageTexture,
    CubeMap
};

class Texture2D
{
public:
    Texture2D() = default;
    Texture2D(const std::string& path, ETextureUsage usage);
    Texture2D(uint32_t width, uint32_t height, ETextureUsage usage);
    ~Texture2D();

    uint32_t GetWidth() const { return m_width; }
    uint32_t GetHeight() const { return m_height; }
    uint32_t GetTexture() const { return m_texture; }

    void Bind(const uint32_t& slot = 0) const;
    void UnBind() const;
private:
    void LoadTexture();
    void LoadCubeMap();
    void CreateFramebufferTexture(uint32_t width, uint32_t height);
    void CreateComputeTexture(uint32_t width, uint32_t height);
    
    std::string m_path;
    bool m_isLoaded = false;
    uint32_t m_width = 0, m_height = 0, m_texture = 0;
    GLenum m_internalFormat, m_dataFormat;
    ETextureUsage m_Usage;
};
