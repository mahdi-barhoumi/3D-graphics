#include <glm/glm.hpp>
#include <glew/glew.hpp>
#include <engine/core/mesh.hpp>
#include <engine/core/camera.hpp>
#include <engine/core/window.hpp>
#include <engine/core/texture.hpp>
#include <engine/core/renderer.hpp>
#include <engine/core/transform.hpp>

namespace Engine
{
    Renderer::Renderer()
    {
        m_Shader = Shader("./assets/shaders/default/vertexShader.glsl", "./assets/shaders/default/fragmentShader.glsl");
        m_GridShader = Shader("./assets/shaders/grid/vertexShader.glsl", "./assets/shaders/grid/fragmentShader.glsl");

        float vertices[] = {
               // Positions        // Colors (RGB)
            0.0f,  0.0f,  0.0f,   1.0f, 0.0f, 0.0f, // X-axis start (Red)
            5.0f,  0.0f,  0.0f,   1.0f, 0.0f, 0.0f, // X-axis end

            0.0f,  0.0f,  0.0f,   0.0f, 1.0f, 0.0f, // Y-axis start (Green)
            0.0f,  5.0f,  0.0f,   0.0f, 1.0f, 0.0f, // Y-axis end

            0.0f,  0.0f,  0.0f,   0.0f, 0.0f, 1.0f, // Z-axis start (Blue)
            0.0f,  0.0f,  5.0f,   0.0f, 0.0f, 1.0f  // Z-axis end
        };
        glGenVertexArrays(1, &gridVAO);
        glGenBuffers(1, &gridVBO);

        glBindVertexArray(gridVAO);
        glBindBuffer(GL_ARRAY_BUFFER, gridVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        glBindBuffer(GL_ARRAY_BUFFER, 0); 
        glBindVertexArray(0);
    }
    void Renderer::Render(World& world, Window& window)
    {
        window.MakeCurrent();
        glViewport(0, 0, window.GetWidth(), window.GetHeight());
        glClearColor((float) 0x79 / 255, (float) 0xA6 / 255, (float) 0xFF / 255, 1.0f);
        glEnable(GL_CULL_FACE);
        glEnable(GL_DEPTH_TEST);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        Handle firstCameraHandle;
        Transform firstCameraTransform;
        Camera firstCamera;

        for (auto [handle, t, c] : world.View<Transform, Camera>())
        {
            firstCameraHandle = handle;
            firstCameraTransform = t;
            firstCamera = c;
            break;
        }

        m_GridShader.Bind();
        glm::mat4 MVP = firstCamera.GetProjectionMatrix(window.GetAspectRatio()) * firstCameraTransform.GetInverseWorldMatrix();
        m_GridShader.SetUniform("MVP", &MVP);

        // TODO: Remove me
        glBindVertexArray(gridVAO);
        glLineWidth(2.0f);
        glDrawArrays(GL_LINES, 0, 6);
        glBindVertexArray(0);

        for (auto [handle, transform, mesh, texture] : world.View<Transform, Mesh, Texture>())
        {
            m_Shader.Bind();
            glm::mat4 vertexPositionTransformationMatrix = firstCamera.GetProjectionMatrix(window.GetAspectRatio()) * firstCameraTransform.GetInverseWorldMatrix() * transform.GetWorldMatrix();
            unsigned int textureUnit = 0;
            texture.Bind(textureUnit);
            m_Shader.SetUniform("sampler", &textureUnit);
            m_Shader.SetUniform("vertexPositionTransformationMatrix", &vertexPositionTransformationMatrix);
            mesh.Draw();
        }
    }
}
