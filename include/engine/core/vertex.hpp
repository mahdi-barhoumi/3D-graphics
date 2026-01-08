#pragma once
#include <glm/glm.hpp>

namespace Engine
{
    struct VertexP3
    {
        glm::vec3 position = glm::vec3(0);
    };
    struct VertexP3C4
    {
        glm::vec3 position = glm::vec3(0);
        glm::vec4 color = glm::vec4(0);
    };
    struct VertexP3N3
    {
        glm::vec3 position = glm::vec3(0);
        glm::vec3 normal = glm::vec3(0);
    };
    struct VertexP3N3C4
    {
        glm::vec3 position = glm::vec3(0);
        glm::vec3 normal = glm::vec3(0);
        glm::vec4 color = glm::vec4(0);
    };
    struct VertexP3T2
    {
        glm::vec3 position = glm::vec3(0);
        glm::vec2 textureCoordinate = glm::vec2(0);
    };
    struct VertexP3T2N3
    {
        glm::vec3 position = glm::vec3(0);
        glm::vec2 textureCoordinate = glm::vec2(0);
        glm::vec3 normal = glm::vec3(0);
    };
}
