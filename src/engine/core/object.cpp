#include <engine/core/object.hpp>

namespace Engine
{
    Object::Object(Handle handle, World* world) : m_Handle(handle), mp_World(world) {}
    bool Object::IsValid() const { return (mp_World && mp_World->m_Registry.valid(m_Handle)); }
    Handle Object::GetHandle() const { return m_Handle; }
}

// #define STB_IMAGE_IMPLEMENTATION
// #include <stb/stb_image.h>

// #include <fstream>
// #include <iostream>
// #include <object.hpp>
// #include <utils.hpp>

// using namespace std;
// using namespace glm;

// object::object(filesystem::path modelFilePath)
// :   vertexArrayID(0), vertexBufferID(0), indexBufferID(0),
//     scale(vec3(1, 1, 1)), position(vec3(0, 0, 0)), rotation(vec3(0, 0, 0)),
//     scalingMatrix(mat4(1)), rotationMatrix(mat4(1)), translationMatrix(mat4(1)), modelMatrix(mat4(1))
// {
//     parseOBJFile(modelFilePath, vertexPositions, vertexTextureCoordinates, vertexNormals, vertexIndices);
//     for (unsigned int i = 0; i < vertexIndices.size(); i++) {
//         indexBuffer.push_back(i);
//         vertexBuffer.push_back(vertexPositions[vertexIndices[i].x - 1].x);
//         vertexBuffer.push_back(vertexPositions[vertexIndices[i].x - 1].y);
//         vertexBuffer.push_back(vertexPositions[vertexIndices[i].x - 1].z);
//         if (vertexIndices[i].y > 0) {
//             vertexBuffer.push_back(vertexTextureCoordinates[vertexIndices[i].y - 1].s);
//             vertexBuffer.push_back(vertexTextureCoordinates[vertexIndices[i].y - 1].t);
//         }
//         else {
//             vertexBuffer.push_back(0);
//             vertexBuffer.push_back(0);
//         }
//         if (vertexIndices[i].z > 0) {
//             vertexBuffer.push_back(vertexNormals[vertexIndices[i].z - 1].x);
//             vertexBuffer.push_back(vertexNormals[vertexIndices[i].z - 1].y);
//             vertexBuffer.push_back(vertexNormals[vertexIndices[i].z - 1].z);
//         }
//         else {
//             vertexBuffer.push_back(0);
//             vertexBuffer.push_back(0);
//             vertexBuffer.push_back(0);
//         }
//     }
// }

// object::object(const object& other)
// :   vertexArrayID(0), vertexBufferID(0), indexBufferID(0),
//     vertexBuffer(other.vertexBuffer), indexBuffer(other.indexBuffer),
//     vertexPositions(other.vertexPositions), vertexTextureCoordinates(other.vertexTextureCoordinates), vertexNormals(other.vertexNormals), vertexIndices(other.vertexIndices),
//     scale(other.scale), position(other.position), rotation(other.rotation),
//     scalingMatrix(other.scalingMatrix), rotationMatrix(other.rotationMatrix), translationMatrix(other.translationMatrix), modelMatrix(other.modelMatrix)
// {
// }

// object::~object()
// {
//     unload();
// }

// const object& object::operator=(const object& other)
// {
//     unload();
//     vertexArrayID = 0;
//     vertexBufferID = 0;
//     indexBufferID = 0;
//     vertexBuffer = other.vertexBuffer;
//     indexBuffer = other.indexBuffer;
//     vertexPositions = other.vertexPositions;
//     vertexTextureCoordinates = other.vertexTextureCoordinates;
//     vertexNormals = other.vertexNormals;
//     vertexIndices = other.vertexIndices;
//     scale = other.scale;
//     position = other.position;
//     rotation = other.rotation;
//     scalingMatrix = other.scalingMatrix;
//     rotationMatrix = other.rotationMatrix;
//     translationMatrix = other.translationMatrix;
//     modelMatrix = other.modelMatrix;
//     return *this;
// }

// void object::load(void)
// {
//     if (!vertexArrayID) {
//         glGenVertexArrays(1, &vertexArrayID);
//         glGenBuffers(1, &vertexBufferID);
//         glGenBuffers(1, &indexBufferID);
//         if (!vertexArrayID || !vertexBufferID || !indexBufferID) {
//             cout << "Error: Could not generate buffers" << endl;
//             exit(-1);
//         }
//         glBindVertexArray(vertexArrayID);
//         glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
//         glBufferData(GL_ARRAY_BUFFER, vertexBuffer.size() * sizeof(GLfloat), &vertexBuffer[0], GL_DYNAMIC_DRAW);
//         glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*) (0 * sizeof(GLfloat)));
//         glEnableVertexAttribArray(0);
//         glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*) (3 * sizeof(GLfloat)));
//         glEnableVertexAttribArray(1);
//         glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*) (5 * sizeof(GLfloat)));
//         glEnableVertexAttribArray(2);
//         glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferID);
//         glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexBuffer.size() * sizeof(GLuint), &indexBuffer[0], GL_DYNAMIC_DRAW);
//         glGenTextures(1, &textureID);
//         glBindTexture(GL_TEXTURE_2D, textureID);
//         int width, height, numberChannels;
//         unsigned char* data = stbi_load("./textures/dirt.bmp", &width, &height, &numberChannels, 0);
//         if (data) {
//             glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
//             glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
//             glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
//             glBindTexture(GL_TEXTURE_2D, 0);
//         }
//         else {
//             cout << "Error: Could not open texture file." << endl;
//             exit(-1);
//         }
//         stbi_image_free(data);
//         glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
//         glBindBuffer(GL_ARRAY_BUFFER, 0);
//         glBindVertexArray(0);
//     }
// }

// void object::unload(void)
// {
//     if (vertexArrayID) {
//         glDeleteVertexArrays(1, &vertexArrayID);
//         glDeleteBuffers(1, &vertexBufferID);
//         glDeleteBuffers(1, &indexBufferID);
//     }
// }

// void object::moveTo(float x, float y, float z)
// {
//     position = vec3(x, y, z);
// }
// void object::moveBy(float dx, float dy, float dz)
// {
//     position += vec3(dx, dy, dz);
// }
// void object::rotate(float degreesX, float degreesY, float degreesZ)
// {
//     rotation += vec3(radians(degreesX), radians(degreesY), radians(degreesZ));
// }
// void object::scaleTo(float factorX, float factorY, float factorZ)
// {
//     if (factorX > 0) scale.x = factorX;
//     if (factorY > 0) scale.y = factorY;
//     if (factorZ > 0) scale.z = factorZ;
// }
// void object::update(void)
// {
//     scalingMatrix = mat4(scale.x, 0, 0, 0, 0, scale.y, 0, 0, 0, 0, scale.z, 0, 0, 0, 0, 1);
//     mat4 rotationXMatrix = mat4(1, 0, 0, 0, 0, cos(rotation.x), sin(rotation.x), 0, 0, -sin(rotation.x), cos(rotation.x), 0, 0, 0, 0, 1);
//     mat4 rotationYMatrix = mat4(cos(rotation.y), 0, -sin(rotation.y), 0, 0, 1, 0, 0, sin(rotation.y), 0, cos(rotation.y), 0, 0, 0, 0, 1);
//     mat4 rotationZMatrix = mat4(cos(rotation.z), sin(rotation.z), 0, 0, -sin(rotation.z), cos(rotation.z), 0, 0, 0, 0, 1, 0, 0, 0, 0, 1);
//     rotationMatrix = rotationZMatrix * rotationYMatrix * rotationXMatrix;
//     translationMatrix = mat4(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, position.x, position.y, position.z, 1);
//     modelMatrix = translationMatrix * rotationMatrix * scalingMatrix;
// }