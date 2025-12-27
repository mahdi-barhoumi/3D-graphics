#pragma once
#include <glm/glm.hpp>

namespace Engine
{
    struct Vertex
    {
        glm::vec3 position = glm::vec3(0);
        glm::vec2 textureCoordinate = glm::vec2(0);
        glm::vec3 normal = glm::vec3(0);
    };
}
