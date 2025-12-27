#pragma once
#include <string>
#include <vector>
#include <glm/glm.hpp>
#include <engine/core/vertex.hpp>

namespace Engine::Utils
{
    void ParseOBJFile(std::string path, std::vector<Engine::Vertex>& verteces, std::vector<glm::uvec3>& indices);
}
