#include <stdexcept>
#include <engine/core/mesh.hpp>
#include <engine/core/utilities.hpp>

namespace Engine
{
    std::unordered_map<std::string, std::weak_ptr<Mesh::Allocation>> Mesh::s_Cache;
    Mesh::Allocation::Allocation(const std::vector<VertexP3C4>& vertices, const std::vector<unsigned int>& indices, Primitive primitive)
    {
        glCreateVertexArrays(1, &VAO);
        glCreateBuffers(1, &VBO);
        glCreateBuffers(1, &IBO);

        if (!VAO || !VBO || !IBO) throw std::runtime_error("Could not generate buffers for mesh.");

        switch (primitive)
        {
            case Primitive::Points: this->primitive = GL_POINTS; break;
            case Primitive::Lines: this->primitive = GL_LINES; break;
            case Primitive::Triangles: this->primitive = GL_TRIANGLES; break;
        }
        indexCount = static_cast<GLsizei>(indices.size());

        glNamedBufferData(IBO, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);
        glNamedBufferData(VBO, vertices.size() * sizeof(VertexP3C4), vertices.data(), GL_STATIC_DRAW);

        constexpr GLuint bindingIndex = 0;
        glVertexArrayElementBuffer(VAO, IBO);
        glVertexArrayVertexBuffer(VAO, bindingIndex, VBO, 0, sizeof(VertexP3C4));
        
        glVertexArrayAttribFormat(VAO, 0, 3, GL_FLOAT, GL_FALSE, offsetof(VertexP3C4, position));
        glVertexArrayAttribBinding(VAO, 0, bindingIndex);
        glEnableVertexArrayAttrib(VAO, 0);

        glVertexArrayAttribFormat(VAO, 1, 4, GL_FLOAT, GL_FALSE, offsetof(VertexP3C4, color));
        glVertexArrayAttribBinding(VAO, 1, bindingIndex);
        glEnableVertexArrayAttrib(VAO, 1);
    }
    Mesh::Allocation::Allocation(const std::vector<VertexP3T2N3>& vertices, const std::vector<unsigned int>& indices, Primitive primitive)
    {
        glCreateVertexArrays(1, &VAO);
        glCreateBuffers(1, &VBO);
        glCreateBuffers(1, &IBO);

        if (!VAO || !VBO || !IBO) throw std::runtime_error("Could not generate buffers for mesh.");

        switch (primitive)
        {
            case Primitive::Points: this->primitive = GL_POINTS; break;
            case Primitive::Lines: this->primitive = GL_LINES; break;
            case Primitive::Triangles: this->primitive = GL_TRIANGLES; break;
        }
        indexCount = static_cast<GLsizei>(indices.size());

        glNamedBufferData(IBO, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);
        glNamedBufferData(VBO, vertices.size() * sizeof(VertexP3T2N3), vertices.data(), GL_STATIC_DRAW);

        constexpr GLuint bindingIndex = 0;
        glVertexArrayElementBuffer(VAO, IBO);
        glVertexArrayVertexBuffer(VAO, bindingIndex, VBO, 0, sizeof(VertexP3T2N3));
        
        glVertexArrayAttribFormat(VAO, 0, 3, GL_FLOAT, GL_FALSE, offsetof(VertexP3T2N3, position));
        glVertexArrayAttribBinding(VAO, 0, bindingIndex);
        glEnableVertexArrayAttrib(VAO, 0);

        glVertexArrayAttribFormat(VAO, 1, 2, GL_FLOAT, GL_FALSE, offsetof(VertexP3T2N3, textureCoordinate));
        glVertexArrayAttribBinding(VAO, 1, bindingIndex);
        glEnableVertexArrayAttrib(VAO, 1);

        glVertexArrayAttribFormat(VAO, 2, 3, GL_FLOAT, GL_FALSE, offsetof(VertexP3T2N3, normal));
        glVertexArrayAttribBinding(VAO, 2, bindingIndex);
        glEnableVertexArrayAttrib(VAO, 2);
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
        
        std::vector<unsigned int> indices;
        std::vector<VertexP3T2N3> vertices;
        if (!Utilities::LoadOBJFile(path, vertices, indices)) throw std::runtime_error("Could not load mesh file.");
        mp_Allocation = std::make_shared<Allocation>(vertices, indices, Primitive::Triangles);
        s_Cache[path] = mp_Allocation;
    }
    Mesh::Mesh(const std::vector<VertexP3C4>& vertices, const std::vector<unsigned int>& indices, Primitive primitive)
    {
        mp_Allocation = std::make_shared<Allocation>(vertices, indices, primitive);
    }
    Mesh::Mesh(const std::vector<VertexP3T2N3>& vertices, const std::vector<unsigned int>& indices, Primitive primitive)
    {
        mp_Allocation = std::make_shared<Allocation>(vertices, indices, primitive);
    }
    size_t Mesh::GetAllocationCount()
    { 
        std::erase_if(s_Cache, [](const auto& item) { return item.second.expired(); });
        return s_Cache.size();
    }
    void Mesh::Draw() const
    {
        glBindVertexArray(GetBackendVAO());
        glDrawElements(GetBackendPrimitive(), GetBackendIndexCount(), GL_UNSIGNED_INT, nullptr);
    }
}
