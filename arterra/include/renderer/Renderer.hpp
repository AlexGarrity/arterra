#pragma once
#include "PCH.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "renderer/Shader.hpp"

namespace arterra {
    class Renderer {
        public:
            Renderer();

            void GenerateVAO();
            void GenerateVBO();
            void GenerateEBO();

            void DrawPoints(std::vector<GLfloat> points, std::vector<GLuint> elements);
            void DrawTestCube();

        private:
            GLuint _vbo;
            GLuint _vao;
            GLuint _ebo;

            Shader _shader;

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
    };


}