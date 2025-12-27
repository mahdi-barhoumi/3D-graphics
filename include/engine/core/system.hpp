#pragma once
#include <type_traits>

namespace Engine
{
    class System
    {
        public:
            virtual void Update(float deltaTime) = 0;
    };

    template <typename T>
    concept SystemConcept = std::is_base_of_v<System, T>;
}
