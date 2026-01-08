#version 460 core

layout (location = 0) in vec3 vertexPosition;
layout (location = 1) in vec2 vertexTextureCoordinate;
layout (location = 2) in vec3 vertexNormal;

out vec3 fragmentPosition;
out vec3 fragmentNormal;
out vec2 fragmentTextureCoordinate;
out vec4 fragmentPositionLightSpace;

uniform mat4 world;
uniform mat4 cameraView;
uniform mat4 cameraProjection;
uniform mat4 lightView;
uniform mat4 lightProjection;

void main()
{    
    fragmentPosition = vec3(world * vec4(vertexPosition, 1.0));
    fragmentNormal = transpose(inverse(mat3(world))) * vertexNormal;
    fragmentTextureCoordinate = vertexTextureCoordinate;
    fragmentPositionLightSpace = lightProjection * lightView * vec4(fragmentPosition, 1.0);

    gl_Position = cameraProjection * cameraView * vec4(fragmentPosition, 1.0);
}