#include <stb/image.hpp>
#include <engine/core/texture.hpp>

namespace Engine
{
    std::unordered_map<std::string, std::weak_ptr<Texture::Allocation>> Texture::s_Cache;
    unsigned int Texture::s_MaxTextureUnits = Texture::Initialize();
    unsigned int Texture::Initialize()
    {
        GLint maxTextureUnits;
        glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &maxTextureUnits);
        return static_cast<unsigned int>(maxTextureUnits);
    }
    Texture::Allocation::Allocation(const std::string& path)
    {
        int width, height;
        unsigned char* data = stbi_load(path.c_str(), &width, &height, nullptr, STBI_rgb_alpha);
        if (!data) throw std::runtime_error("Could not load texture file.");
        glCreateTextures(GL_TEXTURE_2D, 1, &texture);
        if (!texture) throw std::runtime_error("Could not create texture.");
        format = GL_RGBA8;
        glTextureStorage2D(texture, 1, GL_RGBA8, width, height);
        glTextureSubImage2D(texture, 0, 0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glTextureParameteri(texture, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTextureParameteri(texture, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTextureParameteri(texture, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTextureParameteri(texture, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        stbi_image_free(data);
    }
    Texture::Allocation::Allocation(unsigned int width, unsigned int height, Format format)
    {
        glCreateTextures(GL_TEXTURE_2D, 1, &texture);
        if (!texture) throw std::runtime_error("Could not create texture.");
        switch (format)
        {
            case Format::RGB:
                glTextureStorage2D(texture, 1, GL_RGB8, width, height);
                this->format = GL_RGB8;
            break;
            case Format::RGBA:
                glTextureStorage2D(texture, 1, GL_RGBA8, width, height);
                this->format = GL_RGBA8;
            break;
            case Format::Depth:
                glTextureStorage2D(texture, 1, GL_DEPTH_COMPONENT24, width, height);
                this->format = GL_DEPTH_COMPONENT24;
            break;
            case Format::DepthStencil:
                glTextureStorage2D(texture, 1, GL_DEPTH24_STENCIL8, width, height);
                this->format = GL_DEPTH24_STENCIL8;
            break;
        }
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
        mp_Allocation = std::make_shared<Allocation>(path);
        s_Cache[path] = mp_Allocation;
    }
    Texture::Texture(unsigned int width, unsigned int height, Format format) { mp_Allocation = std::make_shared<Allocation>(width, height, format); }
    unsigned int Texture::GetWidth() const
    {
        GLint width;
        glGetTextureParameteriv(mp_Allocation->texture, GL_TEXTURE_WIDTH, &width);
        return static_cast<unsigned int>(width);
    }
    unsigned int Texture::GetHeight() const
    {
        GLint height;
        glGetTextureParameteriv(mp_Allocation->texture, GL_TEXTURE_HEIGHT, &height);
        return static_cast<unsigned int>(height);
    }
    void Texture::SetWrap(Wrap wrap)
    {
        switch (wrap)
        {
            case Wrap::Repeat:
                glTextureParameteri(mp_Allocation->texture, GL_TEXTURE_WRAP_S, GL_REPEAT);
                glTextureParameteri(mp_Allocation->texture, GL_TEXTURE_WRAP_T, GL_REPEAT);
            return;
            case Wrap::ClampToEdge:
                glTextureParameteri(mp_Allocation->texture, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
                glTextureParameteri(mp_Allocation->texture, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            return;
            case Wrap::ClampToBorder:
                glTextureParameteri(mp_Allocation->texture, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
                glTextureParameteri(mp_Allocation->texture, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
            return;
        }
    }
    void Texture::SetMinification(Minification minification)
    {
        switch (minification)
        {
            case Minification::Nearest:
                glTextureParameteri(mp_Allocation->texture, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            return;
            case Minification::Linear:
                glTextureParameteri(mp_Allocation->texture, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            return;
        }
    }
    void Texture::SetMagnification(Magnification magnification)
    {
        switch (magnification)
        {
            case Magnification::Nearest:
                glTextureParameteri(mp_Allocation->texture, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            return;
            case Magnification::Linear:
                glTextureParameteri(mp_Allocation->texture, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            return;
        }
    }
    void Texture::SetBorderColor(const Color& color)
    {
        const GLfloat value[4] = { color.r, color.g, color.b, color.a };
        glTextureParameterfv(mp_Allocation->texture, GL_TEXTURE_BORDER_COLOR, value);
    }
    void Texture::Bind(unsigned int unit) const
    {
        if (s_MaxTextureUnits <= unit) throw std::runtime_error("Tried to bind texture to non existant texture unit.");
        glBindTextureUnit(unit, mp_Allocation->texture);
    }
    unsigned int Texture::GetMaxTextureUnits() { return s_MaxTextureUnits; }
    size_t Texture::GetAllocationCount()
    { 
        std::erase_if(s_Cache, [](const auto& item) { return item.second.expired(); });
        return s_Cache.size();
    }
}
