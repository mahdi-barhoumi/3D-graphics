#pragma once
#include <memory>
#include <string>
#include <unordered_map>
#include <glew/glew.hpp>
#include <engine/core/component.hpp>

namespace Engine
{
    class Mesh : public Component
    {
        private:

        struct Allocation final
        {
            GLuint VAO = 0;
            GLuint VBO = 0;
            GLuint IBO = 0;
            GLsizei indicesCount = 0;

            Allocation(const std::string& path);
            ~Allocation();

            Allocation(const Allocation& other) = delete;
            Allocation(Allocation&& other) = delete;
            Allocation& operator=(const Allocation& other) = delete;
            Allocation& operator=(Allocation&& other) = delete;
        };

        static std::unordered_map<std::string, std::weak_ptr<Allocation>> s_Cache;

        std::shared_ptr<Allocation> mp_Allocation = nullptr;
    
        public:
    
        Mesh() = default;
        Mesh(const Mesh& other) = default;
        Mesh(const std::string& path);
        ~Mesh() = default;
        Mesh& operator=(const Mesh& other) = default;

        void Draw() const;
        void Bind() const;
        static void Unbind();
        static unsigned int GetAllocationCount();
        
    };
}
