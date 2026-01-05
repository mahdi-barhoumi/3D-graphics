#include <stdexcept>
#include <engine/core/framebuffer.hpp>

namespace Engine
{
    Framebuffer::Framebuffer()
    {
        glCreateFramebuffers(1, &m_FBO);
        if (!m_FBO) throw std::runtime_error("Could not create framebuffer.");
    }
    Framebuffer::~Framebuffer() { glDeleteFramebuffers(1, &m_FBO); }
    void Framebuffer::AttachTexture(const Texture& texture)
    {
        switch (texture.mp_Allocation->format)
        {
            case GL_RGB8:
            case GL_RGBA8:
                glNamedFramebufferTexture(m_FBO, GL_COLOR_ATTACHMENT0, texture.mp_Allocation->texture, 0);
            break;
            case GL_DEPTH_COMPONENT24:
                glNamedFramebufferTexture(m_FBO, GL_DEPTH_ATTACHMENT, texture.mp_Allocation->texture, 0);
            break;
            case GL_DEPTH24_STENCIL8:
                glNamedFramebufferTexture(m_FBO, GL_DEPTH_STENCIL_ATTACHMENT, texture.mp_Allocation->texture, 0);
            break;
        }
    }
}