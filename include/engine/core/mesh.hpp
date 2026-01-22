#pragma once
#include <vector>
#include <string>
#include <memory>
#include <unordered_map>
#include <glew/glew.hpp>
#include <engine/core/vertex.hpp>
#include <engine/core/component.hpp>

namespace Engine
{
    class Shader;

    class Mesh : public Component
    {
        friend class Shader;
    
        public:
    
        enum class Primitive
        {
            Points = GL_POINTS,
            Lines = GL_LINES,
            Triangles = GL_TRIANGLES
        };

        Mesh() = default;
        Mesh(const Mesh& other) = default;
        Mesh(const std::string& path);
        Mesh(const std::vector<VertexP3C4>& vertices, const std::vector<unsigned int>& indices, Primitive primitive);
        Mesh(const std::vector<VertexP3T2N3>& vertices, const std::vector<unsigned int>& indices, Primitive primitive);
        ~Mesh() = default;
        Mesh& operator=(const Mesh& other) = default;

        static size_t GetAllocationCount();

        private:

        struct Allocation final
        {
            GLuint VAO;
            GLuint VBO;
            GLuint IBO ;
            GLenum primitive;
            GLsizei indexCount;

            Allocation(const std::vector<VertexP3C4>& vertices, const std::vector<unsigned int>& indices, Primitive primitive);
            Allocation(const std::vector<VertexP3T2N3>& vertices, const std::vector<unsigned int>& indices, Primitive primitive);
            ~Allocation();
        };

        static std::unordered_map<std::string, std::weak_ptr<Allocation>> s_Cache;

        std::shared_ptr<Allocation> mp_Allocation = nullptr;

        void Draw() const;
        inline GLuint GetBackendVAO() const { return mp_Allocation->VAO; }
        inline GLuint GetBackendVBO() const { return mp_Allocation->VBO; }
        inline GLuint GetBackendIBO() const { return mp_Allocation->IBO; }
        inline GLenum GetBackendPrimitive() const { return mp_Allocation->primitive; }
        inline GLsizei GetBackendIndexCount() const { return mp_Allocation->indexCount; }
        
    };
}
