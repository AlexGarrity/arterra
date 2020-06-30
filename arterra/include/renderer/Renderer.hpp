#pragma once
#include "PCH.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace arterra {

    class Renderer {
        public:
            Renderer() {
                // Load GL core using GLAD, if it fails then error and return
                if (!gladLoadGL()) {
                    Logger::Get().Log("GLAD failed to initialise", Logger::Severity::Fatal);
                }

                // Give GLAD the GLFW extension loader function
                gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress));
            }

            void Init()
            {
                glGenVertexArrays(1, &_vao);
                glBindVertexArray(_vao);

                glGenBuffers(1, &_vbo);
                glBindBuffer(GL_ARRAY_BUFFER, _vbo);
                glBufferData(
                    GL_ARRAY_BUFFER, _vertices.size() * sizeof(float), _vertices.data(), GL_STATIC_DRAW
                );

                glGenBuffers(1, &_ebo);
                glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebo);
                glBufferData(
                    GL_ELEMENT_ARRAY_BUFFER, _elements.size() * sizeof(GLuint), _elements.data(), GL_STATIC_DRAW
                );

                // ---
                GLuint vertShader = glCreateShader(GL_VERTEX_SHADER);
                glShaderSource(vertShader, 1, &_vertSource, nullptr);
                glCompileShader(vertShader);
                CheckShaderCompilationStatus(vertShader);

                GLuint fragShader = glCreateShader(GL_FRAGMENT_SHADER);
                glShaderSource(fragShader, 1, &_fragSource, nullptr);
                glCompileShader(fragShader);
                CheckShaderCompilationStatus(fragShader);

                _shaderProgram = glCreateProgram();
                glAttachShader(_shaderProgram, vertShader);
                glAttachShader(_shaderProgram, fragShader);
                glBindFragDataLocation(_shaderProgram, 0, "outColor");
                glLinkProgram(_shaderProgram);
                glUseProgram(_shaderProgram);
                // ---

                _posAttrib = glGetAttribLocation(_shaderProgram, "position");
                glVertexAttribPointer(_posAttrib, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), 0);
                glEnableVertexAttribArray(_posAttrib);

                _colAttrib = glGetAttribLocation(_shaderProgram, "colour");
                glVertexAttribPointer(
                    _colAttrib, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat))
                );
                glEnableVertexAttribArray(_colAttrib);

                _screenRatioAttrib = glGetAttribLocation(_shaderProgram, "screenRatio");
                _projectionUniform = glGetUniformLocation(_shaderProgram, "projection");
                _transformUniform = glGetUniformLocation(_shaderProgram, "transform");
                _viewUniform = glGetUniformLocation(_shaderProgram, "view");
                //glEnableVertexAttribArray(_screenRatioAttrib);

                glVertexAttrib1f(_screenRatioAttrib, 0.5625f);
                glUniformMatrix4fv(_projectionUniform, 1, GL_FALSE, glm::value_ptr(_perspective));
                glUniformMatrix4fv(_viewUniform, 1, GL_FALSE, glm::value_ptr(_view));

                glEnable(GL_DEPTH_TEST);
            }

            void Update()
            {
                glDrawElements(GL_TRIANGLES, _elements.size(), GL_UNSIGNED_INT, nullptr);
                _transform = glm::rotate(_transform, Time::GetDeltaTime() * glm::radians(180.0f), glm::vec3(0.5f, 1.0f, 0.0f));
                glUniformMatrix4fv(_transformUniform, 1, GL_FALSE, glm::value_ptr(_transform));
            }

        private:
            void CheckShaderCompilationStatus(GLuint shader) {
                GLint result;
                char buffer[512];
                glGetShaderiv(shader, GL_COMPILE_STATUS, &result);
                glGetShaderInfoLog(shader, 512, nullptr, buffer);
                if (result == GL_FALSE)
                    std::cout << buffer << std::endl;
                else
                    std::cout << "Everything was fine" << std::endl;
            }

            GLuint _vbo;
            GLuint _vao;
            GLuint _ebo;

            GLuint _shaderProgram;
            GLint _posAttrib;
            GLint _colAttrib;
            GLint _screenRatioAttrib;
            GLint _projectionUniform;
            GLint _transformUniform;
            GLint _viewUniform;

            glm::mat4 _perspective = glm::perspective(glm::radians(45.0f), 1280.0f / 720.0f, 0.1f, 100.0f);
            glm::mat4 _transform = glm::mat4(1.0f);
            glm::mat4 _view = glm::lookAt(
                glm::vec3(0.0f, 0.0f, -3.0f),
                glm::vec3(0.0f, 0.0f, 0.0f),
                glm::vec3(0.0f, 1.0f, 0.0f)
            );

            std::vector<float> _vertices = {
                // Front
                -0.5f,  0.5f, 0.5, 1.0, 0.0, 0.0, // Top-left
                0.5f,  0.5f, 0.5f, 0.0, 1.0, 0.0, // Top-right
                0.5f, -0.5f, 0.5f, 0.0, 0.0, 1.0, // Bottom-right
                -0.5f, -0.5f, 0.5f, 1.0, 1.0, 0.0, // Bottom-left

                // Back
                -0.5f,  0.5f, -0.5f, 0.5, 0.5, 0.0, // Top-left
                0.5f,  0.5f, -0.5f, 0.0, 0.5, 0.5, // Top-right
                0.5f, -0.5f, -0.5f, 0.0, 0.0, 0.0, // Bottom-right
                -0.5f, -0.5f, -0.5f, 1.0, 1.0, 1.0  // Bottom-left
            };

            std::vector<GLuint> _elements = {
                0,3,2,  //Front
                2,1,0,
                1,5,6,	//Right
                6,2,1,
                5,4,7,	//Left
                7,6,5,
                4,7,3,	//Back
                3,0,4,
                4,5,1,	//Top
                1,0,4,
                3,2,6,	//Bottom
                6,7,3,
            };
            
            const char* _fragSource = R"glsl(
            #version 150 core
            in vec3 Colour;
            out vec4 outColor;
            void main()
            {
                outColor = vec4(Colour, 1.0);
            }
            )glsl";

            const char* _vertSource = R"glsl(
            #version 150 core
            in vec3 position;
            in vec3 colour;
            out vec3 Colour;

            uniform mat4 projection;
            uniform mat4 transform;
            uniform mat4 view;
            void main()
            {
                Colour = colour;
                gl_Position = vec4(position, 1.0) * transform * projection * view;
            }
            )glsl";

    };


}