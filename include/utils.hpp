#pragma once

#include <vector>
#include <filesystem>

#include <glm/glm.hpp>

void parseOBJFile(
    std::filesystem::path path,
    std::vector<glm::vec3>& vertexPositions,
    std::vector<glm::vec2>& vertexTextureCoordinates,
    std::vector<glm::vec3>& vertexNormals,
    std::vector<glm::uvec3>& vertexIndices
);