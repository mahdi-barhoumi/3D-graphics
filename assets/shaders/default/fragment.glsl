#version 460 core

in vec3 fragmentPosition;
in vec3 fragmentNormal;
in vec2 fragmentTextureCoordinate;
in vec4 fragmentPositionLightSpace;

out vec4 fragmentColor;

uniform vec3 cameraPosition;
uniform vec3 lightPosition;
uniform vec3 lightColor;

uniform float ambientStrength;

uniform sampler2D diffuseTexture;
uniform sampler2DShadow shadowMap;

float shadow(vec4 fragmentPositionLightSpace)
{
    vec3 projectedCoordinates = fragmentPositionLightSpace.xyz / fragmentPositionLightSpace.w;
    projectedCoordinates = projectedCoordinates * 0.5 + 0.5;
    float bias = 0.001;
    return texture(shadowMap, vec3(projectedCoordinates.xy, projectedCoordinates.z - bias));
}  

void main()
{           
    vec3 diffuseColor = texture(diffuseTexture, fragmentTextureCoordinate).rgb;
    vec3 normal = normalize(fragmentNormal);

    // Ambient
    vec3 ambient = ambientStrength * lightColor;

    // Diffuse
    vec3 lightDirection = normalize(lightPosition - fragmentPosition);
    float diffuseStrength = max(dot(lightDirection, normal), 0.0);
    vec3 diffuse = diffuseStrength * lightColor;

    // Specular
    vec3 cameraDirection = normalize(cameraPosition - fragmentPosition);
    vec3 halfwayDirection = normalize(lightDirection + cameraDirection);  
    float specularStrength = pow(max(dot(normal, halfwayDirection), 0.0), 64.0);
    vec3 specular = specularStrength * lightColor;

    // Shadow
    float shadow = shadow(fragmentPositionLightSpace);     

    // Lighting  
    vec3 lighting = (ambient + shadow * (diffuse + specular)) * diffuseColor;    
    fragmentColor = vec4(lighting, 1.0);
}