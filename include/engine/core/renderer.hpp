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

        inline void ClearDepth() const { glClear(GL_DEPTH_BUFFER_BIT); }
        inline void ClearStencil() const { glClear(GL_STENCIL_BUFFER_BIT); }
        inline void ClearColor(const Color& color) const
        {
            glClearColor(color.r / 255.0f, color.g / 255.0f, color.b / 255.0f, color.a / 255.0f);
            glClear(GL_COLOR_BUFFER_BIT);
        }
        inline void EnableDepthTesting() const { glEnable(GL_DEPTH_TEST); }
        inline void DisableDepthTesting() const { glDisable(GL_DEPTH_TEST); }
        inline void EnableFaceCulling() const { glEnable(GL_CULL_FACE); }
        inline void DisableFaceCulling() const { glDisable(GL_CULL_FACE); }
        inline void DepthTestFunction(DepthTest test) const { glDepthFunc(static_cast<GLenum>(test)); }

    };
}