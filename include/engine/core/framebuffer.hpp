#include <glew/glew.hpp>
#include <engine/core/texture.hpp>

namespace Engine
{
    class Framebuffer
    {
        private:

        GLuint m_FBO;

        public:

        Framebuffer();
        ~Framebuffer();

        void AttachTexture(const Texture& texture);

    };
}