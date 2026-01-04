#version 460 core

layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec2 vertexTextureCoordinate;
layout(location = 2) in vec3 vertexNormal;

uniform mat4 vertexPositionTransformationMatrix;

out vec3 interpolatedVertexNormal;
out vec2 interpolatedVertexTextureCoordinate;

void main()
{
    interpolatedVertexNormal = vertexNormal;
    interpolatedVertexTextureCoordinate = vertexTextureCoordinate;
    gl_Position = vertexPositionTransformationMatrix * vec4(vertexPosition, 1);
}
