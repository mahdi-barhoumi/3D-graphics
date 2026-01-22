#pragma once
#include <concepts>
#include <engine/core/world.hpp>

namespace Engine
{
    class System
    {
        public:

        virtual void Update(World& world, float deltaTime) = 0;

    };

    template <typename T>
    concept SystemConcept = std::derived_from<T, System>
}
