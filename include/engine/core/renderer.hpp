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
            Never = GL_NEVER,
            Less = GL_LESS,
            Equal = GL_EQUAL,
            LessOrEqual = GL_LEQUAL,
            Greater = GL_GREATER,
            NotEqual = GL_NOTEQUAL,
            GreaterOrEqual = GL_GEQUAL,
            Always = GL_ALWAYS
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

        void ClearDepth() const;
        void ClearStencil() const;
        void ClearColor(const Color& color) const;
        void EnableDepthTesting() const;
        void DisableDepthTesting() const;
        void EnableFaceCulling() const;
        void DisableFaceCulling() const;
        void DepthTestFunction(DepthTest test) const;
        void SetLineWidth(float width) const;
    };
}