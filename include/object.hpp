#pragma once

#include <vector>
#include <filesystem>

#include <glew/glew.hpp>
#include <glm/glm.hpp>

#include <collider.hpp>

class object
{
    friend class world;
    friend class renderer;
    private:
        GLuint vertexArrayID, vertexBufferID, indexBufferID, textureID;
        std::vector<GLfloat> vertexBuffer;
        std::vector<GLuint> indexBuffer;
        std::vector<glm::vec3> vertexPositions;
        std::vector<glm::vec2> vertexTextureCoordinates;
        std::vector<glm::vec3> vertexNormals;
        std::vector<glm::uvec3> vertexIndices;
        std::vector<collider> colliders;
        glm::vec3 scale;
        glm::vec3 position;
        glm::vec3 rotation;
        glm::mat4 scalingMatrix;
        glm::mat4 rotationMatrix;
        glm::mat4 translationMatrix;
        glm::mat4 modelMatrix;
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
