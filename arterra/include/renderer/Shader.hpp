#pragma once
#include "PCH.hpp"

#include <glad/glad.h>

namespace arterra {

    class Shader {
        public:
            Shader() = default;

            Shader(std::string vertShaderPath, std::string fragShaderPath) {
                Create(vertShaderPath, fragShaderPath);
            };

            ~Shader() {
                glDeleteProgram(_shaderProgram);
            }

            bool Create(std::string vertPath, std::string fragPath) {
                std::string vertSrc { ReadShaderFile(vertPath) };
                std::string fragSrc { ReadShaderFile(fragPath) };
                
                const char *shaderSrc;

                Logger::Get().Log(vertPath.c_str(), Logger::Severity::Debug);
                _vertShader = glCreateShader(GL_VERTEX_SHADER);
                shaderSrc = vertSrc.data();
                glShaderSource(_vertShader, 1, &shaderSrc, nullptr);
                glCompileShader(_vertShader);
                if (!CheckShaderCompilation(_vertShader))
                    return false;

                Logger::Get().Log(fragPath.c_str(), Logger::Severity::Debug);
                _fragShader = glCreateShader(GL_FRAGMENT_SHADER);
                shaderSrc = fragSrc.data();
                glShaderSource(_fragShader, 1, &shaderSrc, nullptr);
                glCompileShader(_fragShader);
                if (!CheckShaderCompilation(_fragShader))
                    return false;

                _shaderProgram = glCreateProgram();
                glAttachShader(_shaderProgram, _vertShader);
                glAttachShader(_shaderProgram, _vertShader);
                glLinkProgram(_shaderProgram);

                glDeleteShader(_vertShader);
                glDeleteShader(_fragShader);
                return true;
            }


            inline GLuint GetProgram() const { return _shaderProgram; }


        private:
            static std::string ReadShaderFile(std::string path) {
                auto relPath = std::filesystem::current_path() / path;
                if (!std::filesystem::exists(relPath)) {
                    Logger::Get().Log("Shader not found", Logger::Severity::Warning);
                    return "";
                }
                // Read an entire file in two lines because why not
                std::ifstream reader(relPath, std::ios::in);
                return {std::istreambuf_iterator<char>(reader), {}};
            }

            static bool CheckShaderCompilation(GLuint shader) {
                GLint result;
                glGetShaderiv(shader, GL_COMPILE_STATUS, &result);
                if (result == GL_FALSE) {
                    char buffer[512];
                    glGetShaderInfoLog(shader, 512, nullptr, buffer);
                    Logger::Get().Log("Shader failed to compile", Logger::Severity::Warning);
                    Logger::Get().Log(buffer, Logger::Severity::Warning);
                    return false;
                }
                else
                    Logger::Get().Log("Shader compiled successfully", Logger::Severity::Debug);
                return true;
            }

            GLuint _vertShader;
            GLuint _fragShader;
            GLuint _shaderProgram;
    };

};