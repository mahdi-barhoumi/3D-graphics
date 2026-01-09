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
            RGB = GL_RGB8,
            RGBA = GL_RGBA8,
            Depth = GL_DEPTH_COMPONENT24,
            DepthStencil = GL_DEPTH24_STENCIL8,
        };
        enum class Minification
        {
            Nearest = GL_NEAREST,
            Linear = GL_LINEAR
        };
        enum class Magnification
        {
            Nearest = GL_NEAREST,
            Linear = GL_LINEAR
        };
        enum class Wrap
        {
            Repeat = GL_REPEAT,
            ClampToEdge = GL_CLAMP_TO_EDGE,
            ClampToBorder = GL_CLAMP_TO_BORDER
        };
        enum class CompareMode
        {
            None = GL_NONE,
            ReferenceToTexture = GL_COMPARE_REF_TO_TEXTURE
        };
        enum class CompareFunction
        {
            Never = GL_NEVER,
            Less = GL_LESS,
            Equal = GL_EQUAL,
            LessOrEqual = GL_LEQUAL,
            Greater = GL_GREATER,
            NotEqual = GL_NOTEQUAL,
            GreaterOrEqual = GL_GEQUAL,
            Always = GL_ALWAYS
        };

        Texture() = default;
        Texture(const Texture& other) = default;
        Texture(const std::string& path);
        Texture(unsigned int width, unsigned int height, Format format = Format::RGBA);
        ~Texture() = default;
        Texture& operator=(const Texture& other) = default;

        void Bind(unsigned int unit) const;
        void SetWrap(Wrap wrap);
        void SetBorder(const Color& color);
        void SetCompareMode(CompareMode mode);
        void SetCompareFunction(CompareFunction function);
        void SetMinification(Minification minification);
        void SetMagnification(Magnification magnification);
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