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
uniform sampler2D shadowMap;

float shadow(vec4 fragmentPositionLightSpace)
{
    // Perform prespective division to get the fragment's projected position in light space.
    vec3 projectedCoordinates = fragmentPositionLightSpace.xyz / fragmentPositionLightSpace.w;

    // Transform these coordinates to texture space so we can sample the closest depth from the shadow map.
    projectedCoordinates = projectedCoordinates * 0.5 + 0.5;

    if (projectedCoordinates.z > 1.0) return 0.0;

    // Get the closest depth from the light's perspective.
    float closestDepth = texture(shadowMap, projectedCoordinates.xy).r; 

    // Get the depth of the current fragment from the light's perspective.
    float currentDepth = projectedCoordinates.z;

    // Check if it is in shadow with a small bias to reduce shadow acne.
    float bias = 0.001;
    float shadow = currentDepth - bias > closestDepth ? 1.0 : 0.0;

    return shadow;
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
    vec3 lighting = (ambient + (1.0 - shadow) * (diffuse + specular)) * diffuseColor;    
    fragmentColor = vec4(lighting, 1.0);
}