#pragma once
#include <vector>
#include <string>
#include <engine/core/color.hpp>
#include <engine/core/vertex.hpp>

namespace Engine::Utilities
{
    bool LoadOBJFile(std::string path, std::vector<VertexP3T2N3>& vertices, std::vector<unsigned int>& faces);
    bool LoadImageFile(std::string path, unsigned int& width, unsigned int& height, std::vector<Color>& colors);
    bool LoadWAVFile(std::string path /*, ... */);
}
