#pragma once
#include "PCH.hpp"

#include "util/Resource.hpp"

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace arterra {

class ShaderProgram : public DataObject {

    struct Shader : public DataObject {
        enum ShaderType { Fragment,
            Vertex,
            Geometry };
        ShaderType _shaderType;
        GLuint _glID;

        Shader(ShaderType shaderType, std::string path)
            : _shaderType { shaderType }
        {
            // Load the shader resource
            auto loadResult = ResourceManager::Get().Load(path);
            if (!loadResult)
                return;

            // Get a handle to the resource
            auto dataHandle = ResourceManager::Get().GetHandle(path);
            // Get the data from the resource
            auto src = std::string(
                dataHandle._resource->_data.begin(),
                dataHandle._resource->_data.end());
            // Convert uint8_t* to char*
            const char* cSrc = src.c_str();

            switch (shaderType) {
            case ShaderType::Fragment:
                _glID = glCreateShader(GL_FRAGMENT_SHADER);
                break;
            case ShaderType::Vertex:
                _glID = glCreateShader(GL_VERTEX_SHADER);
                break;
            case ShaderType::Geometry:
                _glID = glCreateShader(GL_GEOMETRY_SHADER);
                break;
            }

            glShaderSource(_glID, 1, &cSrc, nullptr);
            glCompileShader(_glID);
        }

        void DumpToLog(std::string title = "Shader") override
        {
            Logger::Get().Log(
                "\t", title,
                " - type: ", _shaderType,
                "; handle:", _glID);
        }

        std::vector<uint8_t> Serialize() override
        {
            return {};
        }
    };

public:
    ShaderProgram() = default;

    // Create a new shader (RAII)
    ShaderProgram(std::string vertShaderPath, std::string fragShaderPath)
    {
        Create(vertShaderPath, fragShaderPath);
    };

    // Delete the program
    ~ShaderProgram()
    {
        glDeleteProgram(_glID);
    }

    // Create a new shader - return whether or not it worked
    bool Create(std::string vertPath, std::string fragPath);

    // Inline returns the program handle
    inline GLuint GetProgram() const { return _glID; }

    // Inlines for setting uniforms in shaders
    inline GLint GetUniform(const char* name)
    {
        return glGetUniformLocation(_glID, name);
    }

    inline void SetUniform(std::string uniformName, glm::mat4 matrix)
    {
        auto uniform = GetUniform(uniformName.c_str());
        glUniformMatrix4fv(uniform, 1, GL_FALSE, glm::value_ptr(matrix));
    }

    inline void SetUniform(std::string uniformName, float_t value)
    {
        auto uniform = GetUniform(uniformName.c_str());
        glUniform1f(uniform, value);
    }

    inline void SetUniform(std::string uniformName, double_t value)
    {
        auto uniform = GetUniform(uniformName.c_str());
        glUniform1d(uniform, value);
    }

    inline void SetUniform(std::string uniformName, glm::vec3 vector)
    {
        auto uniform = GetUniform(uniformName.c_str());
        glUniform3fv(uniform, 1, glm::value_ptr(vector));
    }

    inline void SetUniform(std::string uniformName, glm::vec4 vector)
    {
        auto uniform = GetUniform(uniformName.c_str());
        glUniform4fv(uniform, 1, glm::value_ptr(vector));
    }

    void DumpToLog(std::string title = "ShaderProgram")
    {
        Logger::Get().Log(
            "\t", title,
            " - ", "handle: ", _glID);
    }

private:
    static bool CheckShaderCompilation(GLuint shader);

    std::vector<Shader> _shaders;
    GLuint _glID;
};

};