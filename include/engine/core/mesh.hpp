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

// #pragma once
// #include <string>
// #include <glm/glm.hpp>
// #include <engine/core/resource.hpp>

// namespace Engine
// {
//     struct Vertex
//     {
//         glm::vec3 position = glm::vec3(0);
//         glm::vec2 textureCoordinate = glm::vec2(0);
//         glm::vec3 normal = glm::vec3(0);
//     };

//     class Mesh;

//     template <> 
//     struct ResourceDescriptor<Mesh>
//     {
//         std::string path;
//         bool operator==(const ResourceDescriptor<Mesh>& other) const = default;
//     };

//     class Mesh : public Resource
//     {
//         private:
//             std::vector<Vertex> m_Vertices;
//             std::vector<unsigned int> m_Indices;

//         public:
//             Mesh() = delete;
//             Mesh(ResourceDescriptor<Mesh> descriptor)
//             {

//             }
//             ~Mesh() = default;
//     };
// }

// namespace std
// {
//     template <> 
//     struct hash<Engine::ResourceDescriptor<Engine::Mesh>>
//     {
//         size_t operator()(const Engine::ResourceDescriptor<Engine::Mesh>& description) const noexcept { return hash<string>{}(description.path); }
//     };
// }

// #include <vector>

// #include <glm/glm.hpp>

// struct Face
// {
//     glm::uint vertex1PositionIndex;
//     glm::uint vertex1TextureCoordinateIndex;
//     glm::uint vertex1NormalIndex;
//     glm::uint vertex2PositionIndex;
//     glm::uint vertex2TextureCoordinateIndex;
//     glm::uint vertex2NormalIndex;
//     glm::uint vertex3PositionIndex;
//     glm::uint vertex3TextureCoordinateIndex;
//     glm::uint vertex3NormalIndex;
// };

// class Mesh
// {
//     private:
//         std::vector<glm::vec3> vertexPositions;
//         std::vector<glm::vec2> vertexTextureCoordinates;
//         std::vector<glm::vec3> vertexNormals;
//         std::vector<Face> faces;

//         bool validateFace(Face face);

//     public:
//         Mesh();
//         ~Mesh();

//         void addVertexPosition(glm::vec3 vertexPosition);
//         void addVertexTextureCoordinate(glm::vec2 vertexTextureCoordinate);
//         void addVertexNormal(glm::vec3 vertexNormal);
//         void addFace(Face face);
// };