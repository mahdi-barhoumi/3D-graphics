#pragma once
#include <engine/core/world.hpp>
#include <engine/core/shader.hpp>

namespace Engine
{
    class Window;

    class Renderer
    {
        private:

        Shader m_Shader;
        Shader m_GridShader;

        GLuint gridVBO, gridVAO;

        void Initialize();

        public:

        Renderer();
        ~Renderer() = default;

        void Render(World& world, Window& window);

    };
}
