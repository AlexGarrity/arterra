#pragma once

#include "PCH.hpp"
#include "renderer/Shader.hpp"

namespace arterra {

    class ShaderManager {
        public:
            ShaderManager();

            bool LoadShader(std::string fragPath, std::string vertPath, std::string identifier);
            bool UnloadShader(std::string identifier);
            bool UseShader(std::string identifier);
            GLuint GetShader(std::string identifier);

            inline GLuint ActiveProgram() const { return _activeShader; }


        private:
            std::unordered_map<std::string, Shader> _shaders;
            GLuint _activeShader;

    };


}