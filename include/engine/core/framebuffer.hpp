#include <glew/glew.hpp>

namespace Engine
{
    class Framebuffer
    {
        private:

        GLuint m_Framebuffer;

        public:

        Framebuffer();
        ~Framebuffer();

        void Bind();
        static void Unbind();

    };
}