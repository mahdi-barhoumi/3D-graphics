#include <stdexcept>
#include <engine/core/framebuffer.hpp>

namespace Engine
{
    Framebuffer::Framebuffer()
    {
        glGenFramebuffers(1, &m_Framebuffer);
        if (!m_Framebuffer) throw std::runtime_error("Could not create framebuffer.");
    }
    Framebuffer::~Framebuffer() { glDeleteFramebuffers(1, &m_Framebuffer); }
    void Framebuffer::Bind() { glBindFramebuffer(GL_FRAMEBUFFER, m_Framebuffer); }
    void Framebuffer::Unbind() { glBindFramebuffer(GL_FRAMEBUFFER, 0); }
}