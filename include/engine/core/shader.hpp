#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include <glew/glew.hpp>

namespace Engine
{
    class Shader
    {
        private:

        struct Uniform
        {
            GLenum type;
            GLsizei size = 0;
            GLint location = 0;
        };

        std::unordered_map<std::string, Uniform> m_UniformMap;
        GLuint m_Program = 0;

        public:

        Shader() = default;
        Shader(std::string vertexShaderPath, std::string fragmentShaderPath);
        Shader(std::string vertexShaderPath, std::string fragmentShaderPath, std::string geometryShaderPath);
        ~Shader();
        std::vector<std::string> GetUniformNames() const;
        void SetUniform(const std::string& name, const void*&& value);
        void Bind() const;
        static void Unbind();

    };
}
