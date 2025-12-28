#pragma once
#include <memory>
#include <string>
#include <unordered_map>
#include <glew/glew.hpp>
#include <engine/core/component.hpp>

namespace Engine
{
    class Texture : public Component
    {
        private:
    
        struct Allocation final
        {
            GLuint texture = 0;
            GLuint width = 0;
            GLuint height = 0;

            Allocation(const std::string& path);
            ~Allocation();

            Allocation(const Allocation& other) = delete;
            Allocation(Allocation&& other) = delete;
            Allocation& operator=(const Allocation& other) = delete;
            Allocation& operator=(Allocation&& other) = delete;
        };

        static unsigned int Initialize();
        static unsigned int s_MaxTextureUnits;
        static std::unordered_map<std::string, std::weak_ptr<Allocation>> s_Cache;

        std::shared_ptr<Allocation> mp_Allocation = nullptr;

        public:

        Texture() = default;
        Texture(const Texture& other) = default;
        Texture(const std::string& path);
        ~Texture() = default;
        Texture& operator=(const Texture& other) = default;

        unsigned int GetWidth() const;
        unsigned int GetHeight() const;
        void Bind(unsigned int unit) const;
        static void Unbind();
        static unsigned int GetMaxTextureUnits();
        static unsigned int GetAllocationCount();

    };
}
