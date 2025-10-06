#pragma once
#include <glm/glm.hpp>

using namespace glm;

class light
{
    friend class world;
    friend class renderer;
private:
    vec4 color;
    vec3 position;
    vec3 direction;
public:
    light(vec4 color = vec4(1, 1, 1, 1), vec3 position = vec3(0, 0, 50), vec3 direction = vec3(0, 0, -1));
};
