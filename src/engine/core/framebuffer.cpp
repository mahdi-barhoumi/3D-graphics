#include <stdexcept>
#include <engine/core/framebuffer.hpp>

namespace Engine
{
    Framebuffer Framebuffer::Default = Framebuffer(false);
    Framebuffer::Allocation::Allocation(bool user)
    {
        if (user)
        {
            glCreateFramebuffers(1, &FBO);
            if (!FBO) throw std::runtime_error("Could not create framebuffer.");
            glNamedFramebufferDrawBuffer(FBO, GL_NONE);
            glNamedFramebufferReadBuffer(FBO, GL_NONE);
        }
        else FBO = 0;
    }
    Framebuffer::Allocation::~Allocation() { if (FBO) glDeleteFramebuffers(1, &FBO); }
    Framebuffer::Framebuffer() { mp_Allocation = std::make_shared<Allocation>(true); }
    Framebuffer::Framebuffer(bool user) { mp_Allocation = std::make_shared<Allocation>(user); }
    Framebuffer::Framebuffer(const Texture& texture)
    {
        mp_Allocation = std::make_shared<Allocation>(true);
        AttachTexture(texture);
    }
    void Framebuffer::Bind() const
    {
        glBindFramebuffer(GL_FRAMEBUFFER, GetBackendFBO());
        glViewport(0, 0, GetBackendWidth(), GetBackendHeight());
    }
    void Framebuffer::SetWidth(unsigned int width) { mp_Allocation->width = static_cast<GLsizei>(width); }
    void Framebuffer::SetHeight(unsigned int height) { mp_Allocation->height = static_cast<GLsizei>(height); }
    void Framebuffer::AttachTexture(const Texture& texture)
    {
        if (GetBackendFBO() == 0) return;

        if (GetBackendWidth() && GetBackendWidth() != texture.GetBackendWidth()) throw std::runtime_error("Framebuffer and texture widths don't match.");
        if (GetBackendHeight() && GetBackendHeight() != texture.GetBackendHeight()) throw std::runtime_error("Framebuffer and texture heights don't match.");
        
        SetWidth(texture.GetBackendWidth());
        SetHeight(texture.GetBackendHeight());

        switch (texture.GetBackendFormat())
        {
            case GL_RGB8:
            case GL_RGBA8:
                glNamedFramebufferTexture(GetBackendFBO(), GL_COLOR_ATTACHMENT0, texture.GetBackendTexture(), 0);
                glNamedFramebufferDrawBuffer(GetBackendFBO(), GL_COLOR_ATTACHMENT0);
                glNamedFramebufferReadBuffer(GetBackendFBO(), GL_COLOR_ATTACHMENT0);
            break;
            case GL_DEPTH_COMPONENT24:
                glNamedFramebufferTexture(GetBackendFBO(), GL_DEPTH_ATTACHMENT, texture.GetBackendTexture(), 0);
            break;
            case GL_DEPTH24_STENCIL8:
                glNamedFramebufferTexture(GetBackendFBO(), GL_DEPTH_STENCIL_ATTACHMENT, texture.GetBackendTexture(), 0);
            break;
        }

        if (glCheckNamedFramebufferStatus(GetBackendFBO(), GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) throw std::runtime_error("Wrong framebuffer configuration.");
    }
}