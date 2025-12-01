#pragma once

#include <glm/glm.hpp>

class collider
{
    public:
        virtual glm::vec3 support(glm::vec3 direction);
        bool test(const collider& collider);
};