#pragma once
#include <engine/core/light.hpp>
#include <engine/core/color.hpp>
#include <engine/core/world.hpp>
#include <engine/core/shader.hpp>
#include <engine/core/window.hpp>
#include <engine/core/texture.hpp>
#include <engine/core/transform.hpp>
#include <engine/core/framebuffer.hpp>

namespace Engine
{
    class Window;

    class Renderer
    {
        public:

        enum class DepthTest
        {
            Never,
            Less,
            Equal,
            LessOrEqual,
            Greater,
            NotEqual,
            GreaterOrEqual,
            Always
        };

        Renderer();
        ~Renderer() = default;

        void Render(World& world, Window& window);

        private:

        Shader m_Shader = Shader("./assets/shaders/default/vertex.glsl", "./assets/shaders/default/fragment.glsl");
        Shader m_ShadowShader = Shader("./assets/shaders/shadow/vertex.glsl", "./assets/shaders/shadow/fragment.glsl");
        Shader m_AxisShader = Shader("./assets/shaders/axis/vertex.glsl", "./assets/shaders/axis/fragment.glsl");
        Color m_SkyColor = Color("#79A6FFFF");
        Texture m_ShadowMap = Texture(4096, 4096, Texture::Format::Depth);
        Framebuffer m_ShadowFramebuffer = Framebuffer(m_ShadowMap);

        float m_AmbientStrength = 0.5f;

        Mesh m_AxisMesh;

        Light m_Light = Light();
        Transform m_LightTransform = Transform();

        void ClearDepth();
        void ClearStencil();
        void ClearColor(const Color& color);
        void EnableFaceCulling();
        void DisableFaceCulling();
        void EnableDepthTesting();
        void DisableDepthTesting();
        void DepthTestFunction(DepthTest test);

    };
}