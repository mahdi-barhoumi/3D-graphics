#include <stdexcept>
#include <engine/core/mesh.hpp>
#include <engine/utils/utils.hpp>

namespace Engine
{
    std::unordered_map<std::string, std::weak_ptr<Mesh::Allocation>> Mesh::s_Cache;
    Mesh::Allocation::Allocation(const std::string& path)
    {
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &IBO);

        if (!VAO || !VBO || !IBO) throw std::runtime_error("Could not generate buffers for mesh.");

        std::vector<Vertex> vertices;
        std::vector<glm::uvec3> indices;
        Utils::ParseOBJFile(path, vertices, indices);

        if (!vertices.size() || !indices.size()) throw std::runtime_error("Could not load mesh file.");

        indicesCount = static_cast<GLsizei>(3 * indices.size());

        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(
            GL_ARRAY_BUFFER,
            vertices.size() * sizeof(Vertex),
            vertices.data(),
            GL_STATIC_DRAW
        );

        glVertexAttribPointer(
            0, 3, GL_FLOAT, GL_FALSE,
            sizeof(Vertex),
            (void*) offsetof(Vertex, position)
        );
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(
            1, 2, GL_FLOAT, GL_FALSE,
            sizeof(Vertex),
            (void*) offsetof(Vertex, textureCoordinate)
        );
        glEnableVertexAttribArray(1);

        glVertexAttribPointer(
            2, 3, GL_FLOAT, GL_FALSE,
            sizeof(Vertex),
            (void*) offsetof(Vertex, normal)
        );
        glEnableVertexAttribArray(2);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
        glBufferData(
            GL_ELEMENT_ARRAY_BUFFER,
            indices.size() * sizeof(glm::uvec3),
            indices.data(),
            GL_STATIC_DRAW
        );

        glBindVertexArray(0);
    }
    Mesh::Allocation::~Allocation()
    {
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        glDeleteBuffers(1, &IBO);
    }
    Mesh::Mesh(const std::string& path)
    {
        auto iterator = s_Cache.find(path);
        if (iterator != s_Cache.end() && (mp_Allocation = iterator->second.lock())) return;
        mp_Allocation = std::make_shared<Allocation>(path);
        s_Cache[path] = mp_Allocation;
    }
    size_t Mesh::GetAllocationCount()
    { 
        std::erase_if(s_Cache, [](const auto& item) { return item.second.expired(); });
        return s_Cache.size();
    }
}
