#include <fstream>
#include <filesystem>
#include <unordered_map>
#include <glm/glm.hpp>
#include <stb/image.hpp>
#include <engine/core/utilities.hpp>

namespace Engine::Utilities
{
    bool LoadOBJFile(std::string path, std::vector<VertexP3T2N3>& vertices, std::vector<unsigned int>& indices)
    {
        indices.clear();
        vertices.clear();

        std::filesystem::path filesystemPath(path);
        std::ifstream file(filesystemPath, std::ios::in);
        if (!file || filesystemPath.extension().string() != ".obj")
        {
            file.close();
            return false;
        }
        
        std::string line;
        std::vector<glm::vec3> positions;
        std::vector<glm::vec2> textureCoordinates;
        std::vector<glm::vec3> normals;
        std::unordered_map<size_t, unsigned int> map;

        float x, y, z;
        size_t v1, vt1, vn1, v2, vt2, vn2, v3, vt3, vn3;

        constexpr size_t stride = (8 * sizeof(size_t) / 3);
        constexpr size_t max = 1 << stride;

        auto add = [&](size_t v, size_t vt, size_t vn)
        {
            size_t key = v << (2 * stride) | vt << stride | vn;
            auto iterator = map.find(key);
            if (iterator != map.end()) indices.push_back(map[key]);
            else
            {
                map[key] = vertices.size();
                indices.push_back(vertices.size());
                vertices.push_back({
                    (v) ? positions[v - 1] : glm::vec3(0),
                    (vt) ? textureCoordinates[vt - 1] : glm::vec2(0),
                    (vn) ? normals[vn - 1] : glm::vec3(0),
                });
            }
        };

        while (getline(file, line))
        {
            if (sscanf(line.c_str(), "v %f %f %f", &x, &y, &z) == 3) positions.push_back(glm::vec3(x, y, z));
            else if (sscanf(line.c_str(), "vt %f %f", &x, &y) == 2) textureCoordinates.push_back(glm::vec2(x, y));
            else if (sscanf(line.c_str(), "vn %f %f %f", &x, &y, &z) == 3) normals.push_back(glm::vec3(x, y, z));
            else if (sscanf(line.c_str(), "f %zu/%zu/%zu %zu/%zu/%zu %zu/%zu/%zu", &v1, &vt1, &vn1, &v2, &vt2, &vn2, &v3, &vt3, &vn3) == 9)
            {
                if (v1 > max || vt1 > max || vn1 > max || v2 > max || vt2 > max || vn2 > max || v3 > max || vt3 > max || vn3 > max)
                {
                    vertices.clear();
                    indices.clear();
                    return false;
                }
                add(v1, vt1, vn1);
                add(v2, vt2, vn2);
                add(v3, vt3, vn3);
            }
            else if (sscanf(line.c_str(), "f %zu//%zu %zu//%zu %zu//%zu", &v1, &vn1, &v2, &vn2, &v3, &vn3) == 6)
            {
                if (v1 > max || vn1 > max || v2 > max || vn2 > max || v3 > max || vn3 > max)
                {
                    vertices.clear();
                    indices.clear();
                    return false;
                }
                add(v1, 0, vn1);
                add(v2, 0, vn2);
                add(v3, 0, vn3);
            }
            else if (sscanf(line.c_str(), "f %zu %zu %zu", &v1, &v2, &v3) == 3)
            {
                if (v1 > max || v2 > max || v3 > max)
                {
                    vertices.clear();
                    indices.clear();
                    return false;
                }
                add(v1, 0, 0);
                add(v2, 0, 0);
                add(v3, 0, 0);
            }
        }

        file.close();
        return true;
    }
    bool LoadImageFile(std::string path, unsigned int& width, unsigned int& height, std::vector<Color>& colors)
    {
        int imageWidth, imageHeight;
        unsigned char* data = stbi_load(path.c_str(), &imageWidth, &imageHeight, nullptr, STBI_rgb_alpha);

        if (!data) return false;

        colors.clear();
        colors.resize(imageWidth * imageHeight);

        width = static_cast<unsigned int>(imageWidth);
        height = static_cast<unsigned int>(imageHeight);

        size_t stride;
        size_t count = width * height;
        for (size_t index = 0; index < count; ++index)
        {
            stride = 4 * index;
            colors[index] = Color(
                data[stride    ],
                data[stride + 1],
                data[stride + 2],
                data[stride + 3]
            );
        }

        stbi_image_free(data);
        return true;
    }
    bool LoadWAVFile(std::string path /*, ... */) { return false; }
}