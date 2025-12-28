#pragma once
#include <chrono>

namespace Engine
{
    using Clock = std::chrono::high_resolution_clock;
    using Duration = std::chrono::duration<float>;
}
