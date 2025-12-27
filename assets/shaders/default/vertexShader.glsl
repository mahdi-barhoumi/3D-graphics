#version 460 core

layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec2 vertexTextureCoordinate;
layout(location = 2) in vec3 vertexNormal;

uniform mat4 vertexPositionTransformationMatrix;

out vec2 interpolatedVertexTextureCoordinate;

void main()
{
    interpolatedVertexTextureCoordinate = vertexTextureCoordinate;
    gl_Position = vertexPositionTransformationMatrix * vec4(vertexPosition, 1);
}

// #version 460 core

// layout(location = 0) in vec3 vertexPosition;
// layout(location = 1) in vec2 vertexTextureCoordinate;
// layout(location = 2) in vec3 vertexNormal;

// out vec4 interpolatedVertexNormal;
// out vec2 interpolatedVertexTextureCoordinate;

// uniform mat4 vertexPositionTransformationMatrix;
// uniform mat4 vertexNormalTransformationMatrix;

// void main()
// {
//     interpolatedVertexTextureCoordinate = vertexTextureCoordinate;
//     interpolatedVertexNormal = vertexNormalTransformationMatrix * vec4(vertexNormal, 0);
//     gl_Position = vertexPositionTransformationMatrix * vec4(vertexPosition, 1);
// }