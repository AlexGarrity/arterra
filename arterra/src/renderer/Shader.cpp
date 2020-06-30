#include "renderer/Shader.hpp"

namespace arterra {

    bool ShaderProgram::Create(std::string vertPath, std::string fragPath) {
        
        // Create a vertex shader
        Shader vert {Shader::Vertex, vertPath};
        // Check it compiles ok
        if (!CheckShaderCompilation(vert._ID))
            return false;


        // Create a fragment shader
        Shader frag {Shader::Fragment, fragPath};
        // Check it compiles ok
        if (!CheckShaderCompilation(frag._ID))
            return false;

        // Create a new shader program
        _shaderProgram = glCreateProgram();
        // Attach the two shaders to the program
        glAttachShader(_shaderProgram, vert._ID);
        glAttachShader(_shaderProgram, frag._ID);
        // Bind the fragment data
        glBindFragDataLocation(_shaderProgram, 0, "outColor");
        // Link the program
        glLinkProgram(_shaderProgram);

        // Delete the shaders as they're unnecessary
        glDeleteShader(vert._ID);
        glDeleteShader(frag._ID);
        return true;
    }


    std::string ShaderProgram::ReadShaderFile(std::string path) {
        // Get the shader path
        auto relPath = std::filesystem::current_path() / path;
        // Check it exists
        if (!std::filesystem::exists(relPath)) {
            Logger::Get().Log("Shader not found", Logger::Severity::Warning);
            return "";
        }
        // Read an entire file in two lines because why not
        std::ifstream reader(relPath, std::ios::in);
        return {std::istreambuf_iterator<char>(reader), {}};
    }

    bool ShaderProgram::CheckShaderCompilation(GLuint shader) {
        GLint result;
        char buffer[512];
        // Get the compile status
        glGetShaderiv(shader, GL_COMPILE_STATUS, &result);
        // Get the compile status
        glGetShaderInfoLog(shader, 512, nullptr, buffer);
        Logger::Get().Log(buffer, Logger::Severity::Warning);

        if (result == GL_FALSE) {
            Logger::Get().Log("Shader failed to compile", Logger::Severity::Warning);
            return false;
        }
        else
            Logger::Get().Log("Shader compiled successfully", Logger::Severity::Debug);
        return true;
    }

}