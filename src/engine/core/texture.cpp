#include <stdexcept>
#include <stb/image.hpp>
#include <engine/core/texture.hpp>

namespace Engine
{
    std::unordered_map<std::string, std::weak_ptr<Texture::Allocation>> Texture::s_Cache;
    unsigned int Texture::s_MaxTextureUnits = Texture::Initialize();
    unsigned int Texture::Initialize()
    {
        int maxTextureUnits;
        glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &maxTextureUnits);
        return maxTextureUnits;
    }
    Texture::Allocation::Allocation(const std::string& path)
    {
        int width, height;
        unsigned char* data = stbi_load(path.c_str(), &width, &height, nullptr, STBI_rgb_alpha);
        if (!data) throw std::runtime_error("Could not load texture file.");
        glGenTextures(1, &texture);
        if (!texture) throw std::runtime_error("Could not create texture.");
        this->width = static_cast<GLuint>(width);
        this->height = static_cast<GLuint>(height);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, this->width, this->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glBindTexture(GL_TEXTURE_2D, 0);
        stbi_image_free(data);
    }
    Texture::Allocation::~Allocation() { glDeleteTextures(1, &texture); }
    Texture::Texture(const std::string& path)
    {
        auto iterator = s_Cache.find(path);
        if (iterator != s_Cache.end() && !iterator->second.expired())
        {
            mp_Allocation = iterator->second.lock();
            return;
        }
        mp_Allocation = std::make_shared<Allocation>(path);
        s_Cache[path] = std::weak_ptr<Allocation>(mp_Allocation);
    }
    unsigned int Texture::GetWidth() const { return mp_Allocation->width; }
    unsigned int Texture::GetHeight() const { return mp_Allocation->height; }
    void Texture::Bind(unsigned int unit) const
    {
        if (unit >= s_MaxTextureUnits) throw std::runtime_error("Tried to bind texture to non existant texture unit.");
        glActiveTexture(GL_TEXTURE0 + unit);
        glBindTexture(GL_TEXTURE_2D, mp_Allocation->texture);
    }
    void Texture::Unbind() { glBindTexture(GL_TEXTURE_2D, 0); }
    unsigned int Texture::GetMaxTextureUnits() { return Texture::s_MaxTextureUnits; }
    unsigned int Texture::GetAllocationCount()
    { 
        std::erase_if(s_Cache, [](const auto& item) { return item.second.expired(); });
        return static_cast<unsigned int>(s_Cache.size());
    }
}
