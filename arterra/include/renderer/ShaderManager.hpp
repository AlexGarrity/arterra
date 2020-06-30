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
            void BindAttributes();

            std::unordered_map<std::string, Shader> _shaders;
            GLuint _activeShader;

            GLuint _posAttrib;
            GLuint _colAttrib;

    };


}