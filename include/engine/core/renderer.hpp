#pragma once
#include <engine/core/color.hpp>
#include <engine/core/world.hpp>
#include <engine/core/shader.hpp>
#include <engine/core/window.hpp> // HACK: To make sure the main context exists before creating shaders, find a proper solution.

namespace Engine
{
    class Window;

    class Renderer
    {
        private:

        Color m_ClearColor = Color("#79A6FFFF");
        Shader m_Shader = Shader("./assets/shaders/default/vertex.glsl", "./assets/shaders/default/fragment.glsl");
        Shader m_GridShader = Shader("./assets/shaders/grid/vertex.glsl", "./assets/shaders/grid/fragment.glsl");

        float m_AmbientStrength = 0.25f;

        GLuint gridVBO, gridVAO; // TODO: Remove me.

        public:

        Renderer();
        ~Renderer() = default;

        void Render(World& world, Window& window);

    };
}
