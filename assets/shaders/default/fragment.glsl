#version 460 core

in vec3 interpolatedVertexNormal;
in vec2 interpolatedVertexTextureCoordinate;

uniform sampler2D sampler;
uniform float ambientStrength;

out vec4 FragColor;

void main()
{
    FragColor = texture(sampler, interpolatedVertexTextureCoordinate) * ambientStrength;
}
