#pragma once

#include "PCH.hpp"
#include "renderer/Shader.hpp"

namespace arterra {

class ShaderManager {
public:
    ShaderManager();

    // Load a shader using the give paths and save it to the shader index
    bool LoadShader(std::string fragPath, std::string vertPath, std::string identifier);
    // Unload a shader
    bool UnloadShader(std::string identifier);
    // Set a shader as active
    bool UseShader(std::string identifier);

    // Get a shader ID by identifier
    GLuint GetShader(std::string identifier);
    // Get a shader ID by index position
    GLuint GetShader(std::unordered_map<std::string, ShaderProgram>::iterator it);

    inline ShaderProgram ActiveProgram() const { return _activeShader->second; }

private:
    // Bind attributes
    void BindAttributes();

    // Shader map and reference to active shader
    std::unordered_map<std::string, ShaderProgram> _shaders;
    std::unordered_map<std::string, ShaderProgram>::iterator _activeShader;
};

}