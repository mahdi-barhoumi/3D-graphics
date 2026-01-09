#include <stdexcept>
#include <engine/core/texture.hpp>
#include <engine/core/utilities.hpp>

namespace Engine
{
    std::unordered_map<std::string, std::weak_ptr<Texture::Allocation>> Texture::s_Cache;
    const unsigned int Texture::MaxTextureUnits = Texture::Initialize();
    unsigned int Texture::Initialize()
    {
        GLint maxTextureUnits;
        glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &maxTextureUnits);
        return static_cast<unsigned int>(maxTextureUnits);
    }
    size_t Texture::GetAllocationCount()
    { 
        std::erase_if(s_Cache, [](const auto& item) { return item.second.expired(); });
        return s_Cache.size();
    }
    Texture::Allocation::Allocation(unsigned int width, unsigned int height, Format format)
    {
        glCreateTextures(GL_TEXTURE_2D, 1, &texture);
        if (!texture) throw std::runtime_error("Could not create texture.");
        this->width = static_cast<GLsizei>(width);
        this->height = static_cast<GLsizei>(height);
        switch (format)
        {
            case Format::RGB:
                glTextureStorage2D(texture, 1, GL_RGB8, this->width, this->height);
                this->format = GL_RGB8;
            break;
            case Format::RGBA:
                glTextureStorage2D(texture, 1, GL_RGBA8, this->width, this->height);
                this->format = GL_RGBA8;
            break;
            case Format::Depth:
                glTextureStorage2D(texture, 1, GL_DEPTH_COMPONENT24, this->width, this->height);
                this->format = GL_DEPTH_COMPONENT24;
            break;
            case Format::DepthStencil:
                glTextureStorage2D(texture, 1, GL_DEPTH24_STENCIL8, this->width, this->height);
                this->format = GL_DEPTH24_STENCIL8;
            break;
        }
        glTextureParameteri(texture, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTextureParameteri(texture, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTextureParameteri(texture, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTextureParameteri(texture, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    }
    Texture::Allocation::Allocation(unsigned int width, unsigned int height, Format format, const std::vector<Color>& colors)
    {
        glCreateTextures(GL_TEXTURE_2D, 1, &texture);
        if (!texture) throw std::runtime_error("Could not create texture.");
        this->width = static_cast<GLsizei>(width);
        this->height = static_cast<GLsizei>(height);
        switch (format)
        {
            case Format::RGB:
                glTextureStorage2D(texture, 1, GL_RGB8, this->width, this->height);
                this->format = GL_RGB8;
            break;
            case Format::RGBA:
                glTextureStorage2D(texture, 1, GL_RGBA8, this->width, this->height);
                this->format = GL_RGBA8;
            break;
            case Format::Depth:
                glTextureStorage2D(texture, 1, GL_DEPTH_COMPONENT24, this->width, this->height);
                this->format = GL_DEPTH_COMPONENT24;
            break;
            case Format::DepthStencil:
                glTextureStorage2D(texture, 1, GL_DEPTH24_STENCIL8, this->width, this->height);
                this->format = GL_DEPTH24_STENCIL8;
            break;
        }
        glTextureSubImage2D(texture, 0, 0, 0, this->width, this->height, GL_RGBA, GL_UNSIGNED_BYTE, colors.data());
        glTextureParameteri(texture, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTextureParameteri(texture, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTextureParameteri(texture, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTextureParameteri(texture, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    }
    Texture::Allocation::~Allocation() { glDeleteTextures(1, &texture); }
    Texture::Texture(const std::string& path)
    {
        auto iterator = s_Cache.find(path);
        if (iterator != s_Cache.end() && (mp_Allocation = iterator->second.lock())) return;

        std::vector<Color> colors;
        unsigned int width, height;
        if (!Utilities::LoadImageFile(path, width, height, colors)) std::runtime_error("Could not load texture file.");
        mp_Allocation = std::make_shared<Allocation>(width, height, Format::RGBA, colors);
        s_Cache[path] = mp_Allocation;
    }
    Texture::Texture(unsigned int width, unsigned int height, Format format) { mp_Allocation = std::make_shared<Allocation>(width, height, format); }
    unsigned int Texture::GetWidth() const { return static_cast<unsigned int>(GetBackendWidth()); }
    unsigned int Texture::GetHeight() const { return static_cast<unsigned int>(GetBackendHeight()); }
    void Texture::SetWrap(Wrap wrap)
    {
        switch (wrap)
        {
            case Wrap::Repeat:
                glTextureParameteri(GetBackendTexture(), GL_TEXTURE_WRAP_S, GL_REPEAT);
                glTextureParameteri(GetBackendTexture(), GL_TEXTURE_WRAP_T, GL_REPEAT);
            return;
            case Wrap::ClampToEdge:
                glTextureParameteri(GetBackendTexture(), GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
                glTextureParameteri(GetBackendTexture(), GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            return;
            case Wrap::ClampToBorder:
                glTextureParameteri(GetBackendTexture(), GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
                glTextureParameteri(GetBackendTexture(), GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
            return;
        }
    }
    void Texture::SetBorder(const Color& color)
    {
        const GLfloat value[4] = { color.r / 255.0f, color.g / 255.0f, color.b / 255.0f, color.a / 255.0f };
        glTextureParameterfv(GetBackendTexture(), GL_TEXTURE_BORDER_COLOR, value);
    }
    void Texture::SetMinification(Minification minification)
    {
        switch (minification)
        {
            case Minification::Nearest:
                glTextureParameteri(GetBackendTexture(), GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            return;
            case Minification::Linear:
                glTextureParameteri(GetBackendTexture(), GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            return;
        }
    }
    void Texture::SetMagnification(Magnification magnification)
    {
        switch (magnification)
        {
            case Magnification::Nearest:
                glTextureParameteri(GetBackendTexture(), GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            return;
            case Magnification::Linear:
                glTextureParameteri(GetBackendTexture(), GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            return;
        }
    }
    void Texture::Bind(unsigned int unit) const
    {
        if (MaxTextureUnits <= unit) throw std::runtime_error("Tried to bind texture to non existant texture unit.");
        glBindTextureUnit(static_cast<GLuint>(unit), GetBackendTexture());
    }
}