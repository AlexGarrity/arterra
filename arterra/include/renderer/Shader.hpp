#pragma once
#include "PCH.hpp"

#include <glad/glad.h>

namespace arterra {

    class ShaderProgram {

        struct Shader {
            enum ShaderType {Fragment, Vertex, Geometry};
            ShaderType _shaderType;
            GLuint ID;

            Shader(ShaderType shaderType, std::string path) : _shaderType {shaderType} {
                std::string src = ReadShaderFile(path);
                const char *cSrc = src.data();
                switch (shaderType) {
                    case ShaderType::Fragment:
                        ID = glCreateShader(GL_FRAGMENT_SHADER);
                    break;
                    case ShaderType::Vertex:
                        ID = glCreateShader(GL_VERTEX_SHADER);
                    break;
                    case ShaderType::Geometry:
                        ID = glCreateShader(GL_GEOMETRY_SHADER);
                    break;
                }

                glShaderSource(ID, 1, &cSrc, nullptr);
                glCompileShader(ID);
            }
        };

        public:
            ShaderProgram() = default;

            ShaderProgram(std::string vertShaderPath, std::string fragShaderPath) {
                Create(vertShaderPath, fragShaderPath);
            };

            ~ShaderProgram() {
                glDeleteProgram(_shaderProgram);
            }

            bool Create(std::string vertPath, std::string fragPath) {

                Logger::Get().Log(vertPath.c_str(), Logger::Severity::Debug);
                Shader vert {Shader::Vertex, vertPath};
                if (!CheckShaderCompilation(vert.ID))
                    return false;

                Logger::Get().Log(fragPath.c_str(), Logger::Severity::Debug);
                Shader frag {Shader::Fragment, fragPath};
                if (!CheckShaderCompilation(frag.ID))
                    return false;

                _shaderProgram = glCreateProgram();
                glAttachShader(_shaderProgram, vert.ID);
                glAttachShader(_shaderProgram, frag.ID);
                glBindFragDataLocation(_shaderProgram, 0, "outColor");
                glLinkProgram(_shaderProgram);
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
                char buffer[512];
                glGetShaderiv(shader, GL_COMPILE_STATUS, &result);
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

            std::vector<Shader> _shaders;
            GLuint _shaderProgram;
    };

};