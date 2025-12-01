#include <utils.hpp>
#include <glm/glm.hpp>

#include <vector>
#include <fstream>
#include <iostream>
#include <filesystem>

using namespace std;
using namespace glm;

void parseOBJFile(filesystem::path path, vector<vec3>& vertexPositions, vector<vec2>& vertexTextureCoordinates, vector<vec3>& vertexNormals, vector<uvec3>& vertexIndices)
{
    ifstream file(path, ios::in);
    if (file && path.extension().string() == ".obj") {
        string line;
        while (getline(file, line)) {
            float x, y, z;
            unsigned int v1, vt1, vn1, v2, vt2, vn2, v3, vt3, vn3;
            if (sscanf(line.c_str(), "v %f %f %f", &x, &y, &z) == 3) {
                vertexPositions.push_back(vec3(x, y, z));
            }
            else if (sscanf(line.c_str(), "vt %f %f", &x, &y) == 2) {
                vertexTextureCoordinates.push_back(vec2(x, y));
            }
            else if (sscanf(line.c_str(), "vn %f %f %f", &x, &y, &z) == 3) {
                vertexNormals.push_back(vec3(x, y, z));
            }
            else if (sscanf(line.c_str(), "f %u/%u/%u %u/%u/%u %u/%u/%u", &v1, &vt1, &vn1, &v2, &vt2, &vn2, &v3, &vt3, &vn3) == 9) {
                vertexIndices.push_back(uvec3(v1, vt1, vn1));
                vertexIndices.push_back(uvec3(v2, vt2, vn2));
                vertexIndices.push_back(uvec3(v3, vt3, vn3));
            }
            else if (sscanf(line.c_str(), "f %u//%u %u//%u %u//%u", &v1, &vn1, &v2, &vn2, &v3, &vn3) == 6) {
                vertexIndices.push_back(uvec3(v1, 0, vn1));
                vertexIndices.push_back(uvec3(v2, 0, vn2));
                vertexIndices.push_back(uvec3(v3, 0, vn3));
            }
            else if (sscanf(line.c_str(), "f %u %u %u", &v1, &v2, &v3) == 3) {
                vertexIndices.push_back(uvec3(v1, 0, 0));
                vertexIndices.push_back(uvec3(v2, 0, 0));
                vertexIndices.push_back(uvec3(v3, 0, 0));
            }
        }
        file.close();
    }
    else {
        cout << "Error: Could not open OBJ file " << path.filename().string() << endl;
        file.close();
        exit(-1);
    }
}