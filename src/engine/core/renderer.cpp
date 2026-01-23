#include <glew/glew.hpp>
#include <engine/core/mesh.hpp>
#include <engine/core/object.hpp>
#include <engine/core/camera.hpp>
#include <engine/core/vertex.hpp>
#include <engine/core/renderer.hpp>

namespace Engine
{
    Renderer::Renderer()
    {
        std::vector<VertexP3C4> vertices;
        vertices.push_back({ .position = Vector3(0.0f, 0.0f, 0.0f), .color = Color::Red });
        vertices.push_back({ .position = Vector3(0.1f, 0.0f, 0.0f), .color = Color::Red });
        vertices.push_back({ .position = Vector3(0.0f, 0.0f, 0.0f), .color = Color::Green });
        vertices.push_back({ .position = Vector3(0.0f, 0.1f, 0.0f), .color = Color::Green });
        vertices.push_back({ .position = Vector3(0.0f, 0.0f, 0.0f), .color = Color::Blue });
        vertices.push_back({ .position = Vector3(0.0f, 0.0f, 0.1f), .color = Color::Blue });
        std::vector<unsigned int> indices {0, 1, 2, 3, 4, 5};
        m_AxisMesh = Mesh(vertices, indices, Mesh::Primitive::Lines);

        m_LightTransform.RotateTo(Quaternion(Radians(15.0f), 0.0f, 0.0f));

        m_ShadowMap.SetWrap(Texture::Wrap::ClampToBorder);
        m_ShadowMap.SetBorder(1.0f);
        m_ShadowMap.SetMinification(Texture::Minification::Linear);
        m_ShadowMap.SetMagnification(Texture::Magnification::Linear);
        m_ShadowMap.SetCompareMode(Texture::CompareMode::ReferenceToTexture);
        m_ShadowMap.SetCompareFunction(Texture::CompareFunction::LessOrEqual);
    }
    void Renderer::Render(World& world, Window& window)
    {
        Object cameraObject = world.GetActiveCamera();
        if (!cameraObject.IsValid()) return;
        const Camera& camera = cameraObject.Get<Camera>();
        const Transform& cameraTransform = cameraObject.Get<Transform>();

        window.MakeCurrent();

        Matrix4 vertexPositionTransformationMatrix;
        EnableFaceCulling();
        EnableDepthTesting();
        DepthTestFunction(DepthTest::Less);

        // Shadow pass
        m_ShadowFramebuffer.Bind();
        ClearDepth();

        m_LightTransform.TranslateTo(cameraTransform.GetPosition() + Vector3(0.0f, 0.0f, 10.0f));
        for (auto [handle, transform, mesh, texture] : world.View<Transform, Mesh, Texture>())
        {
            vertexPositionTransformationMatrix = m_Light.GetProjectionMatrix() * m_LightTransform.GetInverseWorldMatrix() * transform.GetWorldMatrix();
            m_ShadowShader.SetUniform("vertexPositionTransformationMatrix", vertexPositionTransformationMatrix);
            m_ShadowShader.Draw(mesh);
        }

        // Second pass
        Framebuffer::Default.Bind();

        EnableDepthTesting();
        EnableFaceCulling();

        ClearColor(m_SkyColor);
        ClearDepth();

        m_Shader.SetUniform("cameraPosition", cameraTransform.GetPosition());
        m_Shader.SetUniform("cameraView", cameraTransform.GetInverseWorldMatrix());
        m_Shader.SetUniform("cameraProjection", camera.GetProjectionMatrix(window.GetAspectRatio()));
        m_Shader.SetUniform("lightPosition", m_LightTransform.GetPosition());
        m_Shader.SetUniform("lightColor", m_Light.GetColor());
        m_Shader.SetUniform("lightView", m_LightTransform.GetInverseWorldMatrix());
        m_Shader.SetUniform("lightProjection", m_Light.GetProjectionMatrix());
        m_Shader.SetUniform("ambientStrength", m_AmbientStrength);
        m_Shader.SetUniform("diffuseTexture", 0);
        m_Shader.SetUniform("shadowMap", 1);
        m_ShadowMap.Bind(1);
        
        for (auto [handle, transform, mesh, texture] : world.View<Transform, Mesh, Texture>())
        {
            texture.Bind(0);
            m_Shader.SetUniform("world", transform.GetWorldMatrix());
            m_Shader.Draw(mesh);
        }

        DepthTestFunction(DepthTest::Always);
        Transform axisTransform = cameraTransform;
        axisTransform.TranslateBy(camera.GetWorldForward(cameraTransform.GetOrientation()));
        vertexPositionTransformationMatrix = camera.GetProjectionMatrix(window.GetAspectRatio()) * cameraTransform.GetInverseWorldMatrix() * axisTransform.GetWorldMatrix();
        m_AxisShader.SetUniform("vertexPositionTransformationMatrix", vertexPositionTransformationMatrix);
        m_AxisShader.Draw(m_AxisMesh);

        window.SwapBuffers();
    }
}
