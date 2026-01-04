#pragma once
#include <vector>
#include <string>
#include <unordered_map>
#include <glew/glew.hpp>
#include <engine/core/mesh.hpp>

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

        GLuint m_Program = 0;
        std::unordered_map<std::string, Uniform> m_UniformMap;

        public:

        Shader() = default;
        Shader(const Shader& other) = delete;
        Shader(Shader&& other) = delete;
        Shader(std::string vertexShaderPath, std::string fragmentShaderPath);
        Shader(std::string vertexShaderPath, std::string fragmentShaderPath, std::string geometryShaderPath);
        ~Shader();
        
        Shader& operator=(const Shader& other) = delete;
        Shader& operator=(Shader&& other) = delete;

        template<typename T>
        void SetUniform(const std::string& name, const T& value)
        {
            const Uniform& uniform = m_UniformMap.at(name);
            if constexpr(std::is_same_v<T, float>) glProgramUniform1fv(m_Program, uniform.location, 1, &value);
            else if constexpr(std::is_same_v<T, glm::vec2>) glProgramUniform2fv(m_Program, uniform.location, 1, &value[0]);
            else if constexpr(std::is_same_v<T, glm::vec3>) glProgramUniform3fv(m_Program, uniform.location, 1, &value[0]);
            else if constexpr(std::is_same_v<T, glm::vec4>) glProgramUniform4fv(m_Program, uniform.location, 1, &value[0]);
            else if constexpr(std::is_same_v<T, int>) glProgramUniform1iv(m_Program, uniform.location, 1, &value);
            else if constexpr(std::is_same_v<T, glm::ivec2>) glProgramUniform2iv(m_Program, uniform.location, 1, &value[0]);
            else if constexpr(std::is_same_v<T, glm::ivec3>) glProgramUniform3iv(m_Program, uniform.location, 1, &value[0]);
            else if constexpr(std::is_same_v<T, glm::ivec4>) glProgramUniform4iv(m_Program, uniform.location, 1, &value[0]);
            else if constexpr(std::is_same_v<T, unsigned int>) glProgramUniform1uiv(m_Program, uniform.location, 1, &value);
            else if constexpr(std::is_same_v<T, glm::uvec2>) glProgramUniform2uiv(m_Program, uniform.location, 1, &value[0]);
            else if constexpr(std::is_same_v<T, glm::uvec3>) glProgramUniform3uiv(m_Program, uniform.location, 1, &value[0]);
            else if constexpr(std::is_same_v<T, glm::uvec4>) glProgramUniform4uiv(m_Program, uniform.location, 1, &value[0]);
            else if constexpr(std::is_same_v<T, glm::mat2>) glProgramUniformMatrix2fv(m_Program, uniform.location, 1, GL_FALSE, &value[0][0]);
            else if constexpr(std::is_same_v<T, glm::mat3>) glProgramUniformMatrix3fv(m_Program, uniform.location, 1, GL_FALSE, &value[0][0]);
            else if constexpr(std::is_same_v<T, glm::mat4>) glProgramUniformMatrix4fv(m_Program, uniform.location, 1, GL_FALSE, &value[0][0]);
            else static_assert("Unsupported uniform type");
        }
        std::vector<std::string> GetUniformNames() const;
        void Draw(const Mesh& mesh) const;

    };
}
