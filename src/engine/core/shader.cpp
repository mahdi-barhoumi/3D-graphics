#include <ranges>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <engine/core/shader.hpp>

namespace Engine
{
    Shader::Shader(std::string vertexShaderPath, std::string fragmentShaderPath)
    {
        m_Program = glCreateProgram();
        if (!m_Program) throw std::runtime_error("Could not create shader program.");
        std::ifstream vertexShaderSourceFile(vertexShaderPath, std::ios::in);
        std::ifstream fragmentShaderSourceFile(fragmentShaderPath, std::ios::in);
        if (!vertexShaderSourceFile || !fragmentShaderSourceFile) throw std::runtime_error("Could not open shader source files.");
        std::string vertexShaderSource, fragmentShaderSource;
        std::stringstream buffer;
        buffer << vertexShaderSourceFile.rdbuf();
        vertexShaderSource = buffer.str();
        buffer.str("");
        buffer << fragmentShaderSourceFile.rdbuf();
        fragmentShaderSource = buffer.str();
        buffer.clear();
        vertexShaderSourceFile.close();
        fragmentShaderSourceFile.close();
        GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
        GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        if (!vertexShader || !fragmentShader) throw std::runtime_error("Could not create shaders.");
        const GLchar* source;
        source = vertexShaderSource.c_str();
        glShaderSource(vertexShader, 1, &source, nullptr);
        source = fragmentShaderSource.c_str();
        glShaderSource(fragmentShader, 1, &source, nullptr);
        glCompileShader(vertexShader);
        glCompileShader(fragmentShader);
        GLint status;
        glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &status);
        if (status == GL_FALSE)
        {
            GLint length;
            glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &length);
            GLchar log[length];
            glGetShaderInfoLog(vertexShader, length, &length, log);
            log[length] = '\0';
            throw std::runtime_error(std::format("Failed to compile vertex shader:\n{}", (char*) log));
        }
        glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &status);
        if (status == GL_FALSE)
        {
            GLint length;
            glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &length);
            GLchar log[length];
            glGetShaderInfoLog(fragmentShader, length, &length, log);
            log[length] = '\0';
            throw std::runtime_error(std::format("Failed to compile fragment shader:\n{}", (char*) log));
        }
        glAttachShader(m_Program, vertexShader);
        glAttachShader(m_Program, fragmentShader);
        glLinkProgram(m_Program);
        glValidateProgram(m_Program);
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
        GLint activeUniformsCount = 0;
        glGetProgramiv(m_Program, GL_ACTIVE_UNIFORMS, &activeUniformsCount);
        GLint maxUniformNameLength;
        glGetProgramiv(m_Program, GL_ACTIVE_UNIFORM_MAX_LENGTH, &maxUniformNameLength);
        for (GLint index = 0; index < activeUniformsCount; ++index)
        {
            Uniform uniform;
            GLsizei length;
            GLchar name[maxUniformNameLength];
            glGetActiveUniform(m_Program, index, maxUniformNameLength, &length, &uniform.size, &uniform.type, name);
            name[length] = '\0';
            uniform.location = glGetUniformLocation(m_Program, name);
            m_UniformMap[std::string(name)] = uniform;
        }
    }
    Shader::Shader(std::string vertexShaderPath, std::string fragmentShaderPath, std::string geometryShaderPath) { throw std::runtime_error("Not implemented."); }
    Shader::~Shader() { glDeleteProgram(m_Program); }
    std::vector<std::string> Shader::GetUniformNames() const { return std::vector<std::string>(m_UniformMap | std::views::keys | std::ranges::to<std::vector>()); }
    void Shader::Draw(const Mesh& mesh) const
    {
        glUseProgram(m_Program);
        mesh.Draw();
    }
}
