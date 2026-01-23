#pragma once
#include <vector>
#include <string>
#include <engine/core/color.hpp>
#include <engine/core/vertex.hpp>

namespace Engine::Utilities
{
    bool LoadOBJFile(const std::string& path, std::vector<VertexP3T2N3>& vertices, std::vector<unsigned int>& faces);
    bool LoadImageFile(const std::string& path, unsigned int& width, unsigned int& height, std::vector<Color>& colors);
    bool LoadOGGFile(const std::string& path, unsigned int& channels, unsigned int& rate, std::vector<short>& pcm);
}