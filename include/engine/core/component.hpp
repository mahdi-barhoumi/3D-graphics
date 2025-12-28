#pragma once

namespace Engine
{
    struct Component {};

    template <typename T>
    concept ComponentConcept = std::derived_from<T, Component> && std::is_copy_constructible_v<T>;
}
