#version 460 core

layout (location = 0) in vec3 vertexPosition;
layout (location = 1) in vec4 vertexColor;

uniform mat4 vertexPositionTransformationMatrix;

out vec4 color;

void main()
{
    gl_Position = vertexPositionTransformationMatrix * vec4(vertexPosition, 1.0);
    color = vertexColor;
}