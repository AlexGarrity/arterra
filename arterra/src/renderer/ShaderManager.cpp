#include "renderer/ShaderManager.hpp"

namespace arterra {

    ShaderManager::ShaderManager() {

    }

    bool ShaderManager::LoadShader(std::string fragPath, std::string vertPath, std::string identifier) {
        _shaders[identifier] = Shader();
        bool result = _shaders[identifier].Create(vertPath, fragPath);
        char buffer[256];
        if (result) {
            snprintf(buffer, 256, "Shader '%s' loaded successfully", identifier.c_str());
            Logger::Get().Log(buffer, Logger::Severity::Debug);
            return true;
        }
        else {
            snprintf(buffer, 256, "Shader '%s' failed to load", identifier.c_str());
            Logger::Get().Log(buffer, Logger::Severity::Warning);
            _shaders.erase(identifier);
            return false;
        }
    }

    bool ShaderManager::UnloadShader(std::string identifier) {
        auto i = _shaders.find(identifier);
        if (i == _shaders.end()) {
            std::cerr << "Tried to unload shader '" << identifier << "' but it doesn't exist" << std::endl;
            return false;
        }

        _shaders.erase(i);
        std::cout << "Unloaded shader '" << identifier << "' successfully" << std::endl;
        return true;
    }

    bool ShaderManager::UseShader(std::string identifier) {
        Logger::Get().Log(identifier.c_str(), Logger::Severity::Debug);
        auto id = GetShader(identifier);
        _activeShader = id;
        glUseProgram(id);
        return true;
    }

    GLuint ShaderManager::GetShader(std::string identifier) {
        auto i = _shaders.find(identifier);
        if (i == _shaders.end()) {
            std::cerr << "Shader '" << identifier << "' does not exist" << std::endl;
            return 0;
        }
        return i->second.GetProgram();
    }

}