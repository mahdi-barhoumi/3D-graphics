#version 460 core

layout (location = 0) in vec3 vertexPosition;

uniform mat4 vertexPositionTransformationMatrix;

void main()
{
    gl_Position = vertexPositionTransformationMatrix * vec4(vertexPosition, 1.0);
}