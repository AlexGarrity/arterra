#include "renderer/ShaderManager.hpp"

namespace arterra {

    ShaderManager::ShaderManager() {

    }

    // Load a shader using the given paths and save it to the index
    bool ShaderManager::LoadShader(std::string fragPath, std::string vertPath, std::string identifier) {
        // Create a new shader program
        _shaders[identifier] = ShaderProgram();
        // Create the shader
        bool result = _shaders[identifier].Create(vertPath, fragPath);
        char buffer[256];
        // Check if it worked or not
        if (result) {
            Logger::Get().Log(Logger::Debug, "Shader [", identifier, "] loaded successfully");
            return true;
        }
        else {
            snprintf(buffer, 256, "Shader '%s' failed to load", identifier.c_str());
            Logger::Get().Log(Logger::Warning, "Shader [", identifier, "] failed to load");
            // It didn't work so don't save it
            _shaders.erase(identifier);
            return false;
        }
    }

    // Delete a shader from the index
    bool ShaderManager::UnloadShader(std::string identifier) {
        // Find the requested shader
        auto i = _shaders.find(identifier);
        // Shader doesn't exist anyway
        if (i == _shaders.end()) {
            std::cerr << "Tried to unload shader '" << identifier << "' but it doesn't exist" << std::endl;
            return false;
        }

        // Delete the shader
        _shaders.erase(i);
        std::cout << "Unloaded shader '" << identifier << "' successfully" << std::endl;
        return true;
    }

    bool ShaderManager::UseShader(std::string identifier) {
        // Set the active shader to the one specified
        _activeShader = _shaders.find(identifier);
        // Get its ID
        auto id = GetShader(_activeShader);
        // Use the program
        glUseProgram(id);
        // Bind attributes to the shader
        BindAttributes();
        return true;
    }

    GLuint ShaderManager::GetShader(std::string identifier) {
        // Find the shader requested
        auto i = _shaders.find(identifier);
        if (i == _shaders.end()) {
            // It doesn't exist
            std::cerr << "Shader '" << identifier << "' does not exist" << std::endl;
            return 0;
        }
        // Return the shader ID
        return i->second.GetProgram();
    }

    GLuint ShaderManager::GetShader(std::unordered_map<std::string, ShaderProgram>::iterator it) {
        // Return the shader ID
        return it->second.GetProgram();
    }

    void ShaderManager::BindAttributes() {
        // Get the handle for the position attribute
        GLuint posAttrib = glGetAttribLocation(ActiveProgram().GetProgram(), "position");
        // Describe how to handle the data given
        glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), 0);
        // Enable the attribute
        glEnableVertexAttribArray(posAttrib);

        // Same but for colour
        GLuint colAttrib = glGetAttribLocation(ActiveProgram().GetProgram(), "color");
        glVertexAttribPointer(colAttrib, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
        glEnableVertexAttribArray(colAttrib);
    }

}