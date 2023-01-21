#pragma once
#include "Texture.h"

typedef unsigned GLuint;

class Framebuffer
{
public:
    Framebuffer(uint32_t width, uint32_t height);
    ~Framebuffer();
    unsigned int GetTexture() { return m_texture->GetTexture(); }
    void ResizeFrameBuffer(uint32_t width, uint32_t height) const;
    void Bind() const;
    void UnBind() const;

private:
    GLuint m_framebuffer = 0, m_renderbuffer = 0;
    Texture2D* m_texture;
};
