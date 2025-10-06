#pragma once
#include <vector>
#include <filesystem>
#include <glm/glm.hpp>
#include <glew/glew.hpp>

using namespace glm;

class world;
class renderer;

class object
{
    friend class world;
    friend class renderer;
private:
    GLuint vertexArrayID, vertexBufferID, indexBufferID, textureID;
    std::vector<GLfloat> vertexBuffer;
    std::vector<GLuint> indexBuffer;
    std::vector<vec3> vertexPositions;
    std::vector<vec2> vertexTextureCoordinates;
    std::vector<vec3> vertexNormals;
    std::vector<uvec3> vertexIndices;
    vec3 scale;
    vec3 position;
    vec3 rotation;
    mat4 scalingMatrix;
    mat4 rotationMatrix;
    mat4 translationMatrix;
    mat4 modelMatrix;
public:
    object(std::filesystem::path modelFilePath);
    object(const object& object);
    ~object();
    const object& operator=(const object& object);
    void load(void);
    void unload(void);
    void moveTo(float x, float y, float z);
    void moveBy(float dx, float dy, float dz);
    void rotate(float degreesX, float degreesY, float degreesZ);
    void scaleTo(float factorX, float factorY, float factorZ);
    void update(void);
};
