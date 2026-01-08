#include <memory>
#include <glew/glew.hpp>
#include <engine/core/texture.hpp>

namespace Engine
{
    class Framebuffer
    {
        friend class Window;

        private:

        struct Allocation final
        {
            GLuint FBO;
            GLsizei width = 0;
            GLsizei height = 0;

            Allocation(bool user = true);
            ~Allocation();
        };

        Framebuffer(bool user);

        void SetWidth(unsigned int width);
        void SetHeight(unsigned int height);
        inline GLuint GetBackendFBO() const { return mp_Allocation->FBO; }
        inline GLuint GetBackendWidth() const { return mp_Allocation->width; }
        inline GLuint GetBackendHeight() const { return mp_Allocation->height; }

        std::shared_ptr<Allocation> mp_Allocation = nullptr;

        public:

        Framebuffer();
        Framebuffer(const Texture& texture);
        ~Framebuffer() = default;

        void Bind() const;
        void AttachTexture(const Texture& texture);

        static Framebuffer Default;

    };
}