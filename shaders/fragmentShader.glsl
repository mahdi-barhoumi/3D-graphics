#version 460 core

smooth in vec4 interpolatedVertexNormal;
in vec2 interpolatedVertexTextureCoordinate;

out vec4 FragColor;

uniform sampler2D sampler;
uniform mat4 lightTransformationMatrix;

vec4 lightSource = vec4(0.0, 0.0, -1.0, 0);
 
void main()
{
    FragColor = texture(sampler, interpolatedVertexTextureCoordinate) * max(0.25, dot(lightTransformationMatrix * lightSource, -vec4(interpolatedVertexNormal)));
}