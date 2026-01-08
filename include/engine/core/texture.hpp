#pragma once
#include <vector>
#include <memory>
#include <string>
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

        static size_t GetAllocationCount();
        static const unsigned int MaxTextureUnits;

        private:

        struct Allocation final
        {
            GLuint texture;
            GLenum format;
            GLsizei width;
            GLsizei height;

            Allocation(unsigned int width, unsigned int height, Format format);
            Allocation(unsigned int width, unsigned int height, Format format, const std::vector<Color>& colors);
            ~Allocation();
        };

        static unsigned int Initialize();
        static std::unordered_map<std::string, std::weak_ptr<Allocation>> s_Cache;

        std::shared_ptr<Allocation> mp_Allocation = nullptr;

        inline GLuint GetBackendTexture() const { return mp_Allocation->texture; }
        inline GLenum GetBackendFormat() const { return mp_Allocation->format; }
        inline GLsizei GetBackendWidth() const { return mp_Allocation->width; }
        inline GLsizei GetBackendHeight() const { return mp_Allocation->height; }

    };
}