#include <glm/glm.hpp>
#include <glew/glew.hpp>
#include <engine/core/mesh.hpp>
#include <engine/core/camera.hpp>
#include <engine/core/vertex.hpp>
#include <engine/core/texture.hpp>
#include <engine/core/renderer.hpp>
#include <engine/core/transform.hpp>

namespace Engine
{
    Renderer::Renderer()
    {
        std::vector<VertexP3C4> vertices;
        vertices.push_back({ .position = glm::vec3(0.0f, 0.0f, 0.0f), .color = Color::Red });
        vertices.push_back({ .position = glm::vec3(5.0f, 0.0f, 0.0f), .color = Color::Red });
        vertices.push_back({ .position = glm::vec3(0.0f, 0.0f, 0.0f), .color = Color::Green });
        vertices.push_back({ .position = glm::vec3(0.0f, 5.0f, 0.0f), .color = Color::Green });
        vertices.push_back({ .position = glm::vec3(0.0f, 0.0f, 0.0f), .color = Color::Blue });
        vertices.push_back({ .position = glm::vec3(0.0f, 0.0f, 5.0f), .color = Color::Blue });
        std::vector<unsigned int> indices {0, 1, 2, 3, 4, 5};
        m_AxisMesh = Mesh(vertices, indices, Mesh::Primitive::Lines);

        m_Shader.SetUniform("ambientStrength", m_AmbientStrength);
    }
    void Renderer::Render(World& world, Window& window)
    {
        window.MakeCurrent();
        glViewport(0, 0, window.GetWidth(), window.GetHeight());
        glClearColor(m_ClearColor.r, m_ClearColor.g, m_ClearColor.b, m_ClearColor.a);
        glEnable(GL_CULL_FACE);
        glEnable(GL_DEPTH_TEST);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        Handle firstCameraHandle;
        Transform firstCameraTransform;
        Camera firstCamera;

        for (auto [handle, transform, camera] : world.View<Transform, Camera>())
        {
            firstCameraHandle = handle;
            firstCameraTransform = transform;
            firstCamera = camera;
            break;
        }

        glm::mat4 vertexPositionTransformationMatrix =  firstCamera.GetProjectionMatrix(window.GetAspectRatio()) * firstCameraTransform.GetInverseWorldMatrix();
        m_AxisShader.SetUniform("vertexPositionTransformationMatrix", vertexPositionTransformationMatrix);
        m_AxisShader.Draw(m_AxisMesh);

        for (auto [handle, transform, mesh, texture] : world.View<Transform, Mesh, Texture>())
        {
            vertexPositionTransformationMatrix = firstCamera.GetProjectionMatrix(window.GetAspectRatio()) * firstCameraTransform.GetInverseWorldMatrix() * transform.GetWorldMatrix();
            texture.Bind(0);
            m_Shader.SetUniform("sampler", 0);
            m_Shader.SetUniform("vertexPositionTransformationMatrix", vertexPositionTransformationMatrix);
            m_Shader.Draw(mesh);
        }
    }
}
