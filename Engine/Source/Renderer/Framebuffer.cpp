#include "pch.h"

#include "Framebuffer.h"

#include <glad/glad.h>

#include "Core/Core.h"

Framebuffer::Framebuffer(uint32_t width, uint32_t height)
{
    glCreateFramebuffers(1, &m_framebuffer);

    m_texture = new Texture2D(width, height, ETextureUsage::FramebufferTexture);
    m_texture->Bind();
    glNamedFramebufferTexture(m_framebuffer, GL_COLOR_ATTACHMENT0, m_texture->GetTexture(), 0);

    glCreateRenderbuffers(1, &m_renderbuffer);
    glNamedRenderbufferStorage(m_renderbuffer, GL_DEPTH24_STENCIL8, (int)width, (int)height);
    glNamedFramebufferRenderbuffer(m_framebuffer, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_renderbuffer);

#ifdef _DEBUG
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        LOG_ERROR("Framebuffer is incomplete")
#endif
}

Framebuffer::~Framebuffer()
{
    glDeleteFramebuffers(1, &m_framebuffer);
    glDeleteRenderbuffers(1, &m_renderbuffer);
}

void Framebuffer::ResizeFrameBuffer(uint32_t width, uint32_t height) const
{
    glBindTexture(GL_TEXTURE_2D, m_texture->GetTexture());
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, (int)width, (int)height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_texture->GetTexture(), 0);

    glBindRenderbuffer(GL_RENDERBUFFER, m_renderbuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, (int)width, (int)height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_renderbuffer);
}

void Framebuffer::Bind() const
{
    glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffer);
}

void Framebuffer::UnBind() const
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}




