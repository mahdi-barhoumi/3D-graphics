#pragma once
#include <string>
#include <memory>
#include <unordered_map>
#include <glew/glew.hpp>
#include <engine/core/color.hpp>
#include <engine/core/component.hpp>

namespace Engine
{
    class Texture : public Component
    {
        friend class Framebuffer;

        public:

        enum class Format
        {
            RGB,
            RGBA,
            Depth,
            DepthStencil,
        };
        enum class Minification
        {
            Nearest,
            Linear
        };
        enum class Magnification
        {
            Nearest,
            Linear
        };
        enum class Wrap
        {
            Repeat,
            ClampToEdge,
            ClampToBorder
        };

        Texture() = default;
        Texture(const Texture& other) = default;
        Texture(const std::string& path);
        Texture(unsigned int width, unsigned int height, Format format = Format::RGBA);
        ~Texture() = default;
        Texture& operator=(const Texture& other) = default;

        void Bind(unsigned int unit) const;
        void SetWrap(Wrap wrap);
        void SetMinification(Minification minification);
        void SetMagnification(Magnification magnification);
        void SetBorderColor(const Color& color);
        unsigned int GetWidth() const;
        unsigned int GetHeight() const;
        static unsigned int GetMaxTextureUnits();
        static size_t GetAllocationCount();

        private:

        struct Allocation final
        {
            GLuint texture;
            GLenum format;

            Allocation(const std::string& path);
            Allocation(unsigned int width, unsigned int height, Format format);
            ~Allocation();
        };

        static unsigned int Initialize();
        static unsigned int s_MaxTextureUnits;
        static std::unordered_map<std::string, std::weak_ptr<Allocation>> s_Cache;

        std::shared_ptr<Allocation> mp_Allocation;

    };
}
