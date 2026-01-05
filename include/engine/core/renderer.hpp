#pragma once
#include <engine/core/color.hpp>
#include <engine/core/world.hpp>
#include <engine/core/shader.hpp>
#include <engine/core/window.hpp>

namespace Engine
{
    class Window;

    class Renderer
    {
        private:

        Color m_ClearColor = Color("#79A6FFFF");
        Shader m_Shader = Shader("./assets/shaders/default/vertex.glsl", "./assets/shaders/default/fragment.glsl");
        Shader m_AxisShader = Shader("./assets/shaders/axis/vertex.glsl", "./assets/shaders/axis/fragment.glsl");
        Mesh m_AxisMesh;

        float m_AmbientStrength = 0.25f;

        public:

        Renderer();
        ~Renderer() = default;

        void Render(World& world, Window& window);

    };
}
