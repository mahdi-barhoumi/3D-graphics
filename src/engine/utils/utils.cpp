#include <map>
#include <vector>
#include <fstream>
#include <filesystem>
#include <glm/glm.hpp>
#include <engine/core/vertex.hpp>
#include <engine/utils/utils.hpp>

namespace Engine::Utils
{
    using namespace std;
    using namespace glm;

    void ParseOBJFile(string path, vector<Vertex>& verteces, vector<uvec3>& indices)
    {
        if (verteces.size() || indices.size()) throw runtime_error("Vectors not empty.");
        filesystem::path filesystemPath = filesystem::path(path);
        ifstream file(filesystemPath, ios::in);
        if (!file || filesystemPath.extension().string() != ".obj") throw runtime_error("Could not open mesh file.");
        string line;
        uint64_t key;
        uvec3 faceIndices;
        vector<vec3> vertexPositions;
        vector<uvec2> vertexTextureCoordinates;
        vector<vec3> vertexNormals;
        map<uint64_t, unsigned int> map;
        while (getline(file, line))
        {
            const unsigned int maxVertexIndex = (1 << 21);
            float x, y, z;
            unsigned int v1, vt1, vn1, v2, vt2, vn2, v3, vt3, vn3;
            if (sscanf(line.c_str(), "v %f %f %f", &x, &y, &z) == 3)
            {
                vertexPositions.push_back(vec3(x, y, z));
            }
            else if (sscanf(line.c_str(), "vt %f %f", &x, &y) == 2)
            {
                vertexTextureCoordinates.push_back(vec2(x, y));
            }
            else if (sscanf(line.c_str(), "vn %f %f %f", &x, &y, &z) == 3)
            {
                vertexNormals.push_back(vec3(x, y, z));
            }
            else if (sscanf(line.c_str(), "f %u/%u/%u %u/%u/%u %u/%u/%u", &v1, &vt1, &vn1, &v2, &vt2, &vn2, &v3, &vt3, &vn3) == 9)
            {
                if (v1 > maxVertexIndex || vt1 > maxVertexIndex || vn1 > maxVertexIndex ||
                    v2 > maxVertexIndex || vt2 > maxVertexIndex || vn2 > maxVertexIndex ||
                    v3 > maxVertexIndex || vt3 > maxVertexIndex || vn3 > maxVertexIndex)
                {
                    throw runtime_error("Can't parse OBJ file with more than 2097152 distinct vertex attributes.");
                }
                key = (uint64_t(v1 - 1) << 42) | (uint64_t(vt1 - 1) << 21) | uint64_t(vn1 - 1);
                if (map.contains(key)) faceIndices.x = map[key];
                else
                {
                    verteces.push_back(Vertex{
                        .position = vertexPositions[v1 - 1],
                        .textureCoordinate = vertexTextureCoordinates[vt1 - 1],
                        .normal = vertexNormals[vn1 - 1]
                    });
                    map[key] = faceIndices.x = verteces.size() - 1;
                }
                key = (uint64_t(v2 - 1) << 42) | (uint64_t(vt2 - 1) << 21) | uint64_t(vn2 - 1);
                if (map.contains(key)) faceIndices.y = map[key];
                else
                {
                    verteces.push_back(Vertex{
                        .position = vertexPositions[v2 - 1],
                        .textureCoordinate = vertexTextureCoordinates[vt2 - 1],
                        .normal = vertexNormals[vn2 - 1]
                    });
                    map[key] = faceIndices.y = verteces.size() - 1;
                }
                key = (uint64_t(v3 - 1) << 42) | (uint64_t(vt3 - 1) << 21) | uint64_t(vn3 - 1);
                if (map.contains(key)) faceIndices.z = map[key];
                else
                {
                    verteces.push_back(Vertex{
                        .position = vertexPositions[v3 - 1],
                        .textureCoordinate = vertexTextureCoordinates[vt3 - 1],
                        .normal = vertexNormals[vn3 - 1]
                    });
                    map[key] = faceIndices.z = verteces.size() - 1;
                }
                indices.push_back(faceIndices);
            }
            else if (sscanf(line.c_str(), "f %u//%u %u//%u %u//%u", &v1, &vn1, &v2, &vn2, &v3, &vn3) == 6)
            {
                if (v1 > maxVertexIndex || vn1 > maxVertexIndex ||
                    v2 > maxVertexIndex || vn2 > maxVertexIndex ||
                    v3 > maxVertexIndex || vn3 > maxVertexIndex)
                {
                    throw runtime_error("Can't parse OBJ file with more than 2097152 distinct vertex attributes.");
                }
                uvec3 faceIndices;
                key = (uint64_t(v1 - 1) << 42) | uint64_t(vn1 - 1);
                if (map.contains(key)) faceIndices.x = map[key];
                else
                {
                    verteces.push_back(Vertex{
                        .position = vertexPositions[v1 - 1],
                        .textureCoordinate = vec2(0),
                        .normal = vertexNormals[vn1 - 1]
                    });
                    map[key] = faceIndices.x = verteces.size() - 1;
                }
                key = (uint64_t(v2 - 1) << 42) | uint64_t(vn2 - 1);
                if (map.contains(key)) faceIndices.y = map[key];
                else
                {
                    verteces.push_back(Vertex{
                        .position = vertexPositions[v2 - 1],
                        .textureCoordinate = vec2(0),
                        .normal = vertexNormals[vn2 - 1]
                    });
                    map[key] = faceIndices.y = verteces.size() - 1;
                }
                key = (uint64_t(v3 - 1) << 42) | uint64_t(vn3 - 1);
                if (map.contains(key)) faceIndices.z = map[key];
                else
                {
                    verteces.push_back(Vertex{
                        .position = vertexPositions[v3 - 1],
                        .textureCoordinate = vec2(0),
                        .normal = vertexNormals[vn3 - 1]
                    });
                    map[key] = faceIndices.z = verteces.size() - 1;
                }
                indices.push_back(faceIndices);
            }
            else if (sscanf(line.c_str(), "f %u %u %u", &v1, &v2, &v3) == 3)
            {
                if (v1 > maxVertexIndex || v2 > maxVertexIndex || v3 > maxVertexIndex)
                {
                    throw runtime_error("Can't parse OBJ file with more than 2097152 distinct vertex attributes.");
                }
                uvec3 faceIndices;
                key = uint64_t(v1 - 1) << 42;
                if (map.contains(key)) faceIndices.x = map[key];
                else
                {
                    verteces.push_back(Vertex{
                        .position = vertexPositions[v1 - 1],
                        .textureCoordinate = vec2(0),
                        .normal = vec3(0)
                    });
                    map[key] = faceIndices.x = verteces.size() - 1;
                }
                key = uint64_t(v2 - 1) << 42;
                if (map.contains(key)) faceIndices.y = map[key];
                else
                {
                    verteces.push_back(Vertex{
                        .position = vertexPositions[v2 - 1],
                        .textureCoordinate = vec2(0),
                        .normal = vec3(0)
                    });
                    map[key] = faceIndices.y = verteces.size() - 1;
                }
                key = uint64_t(v3 - 1) << 42;
                if (map.contains(key)) faceIndices.z = map[key];
                else
                {
                    verteces.push_back(Vertex{
                        .position = vertexPositions[v3 - 1],
                        .textureCoordinate = vec2(0),
                        .normal = vec3(0)
                    });
                    map[key] = faceIndices.z = verteces.size() - 1;
                }
                indices.push_back(faceIndices);
            }
        }
        file.close();
    }
}