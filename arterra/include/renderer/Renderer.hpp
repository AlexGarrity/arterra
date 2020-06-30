#pragma once
#include "PCH.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "renderer/ShaderManager.hpp"
#include "renderer/Camera.hpp"

namespace arterra {
    class Renderer {
        public:
            Renderer(Camera &camera);

            // Generate a new vertex array object
            void GenerateVAO();
            // Generate a new vertex buffer object
            void GenerateVBO();
            // Generate a new element buffer object
            void GenerateEBO();

            // Update the renderer to account for input
            void Update();

            // Draw point using elements
            void DrawPoints(std::vector<GLfloat> &points, std::vector<GLuint> &elements);
            // Draw a rainbow cube
            void DrawTestCube();

        private:
            GLuint _vbo;
            GLuint _vao;
            GLuint _ebo;

            ShaderManager _shaderManager;
            Camera *_camera;

            GLint _viewProjectionUniform;


            // Test cube
            std::vector<GLfloat> _vertices = {
                -0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f,	// Front Top Left		- Red	- 0
                0.5f,  0.5f, 0.5f, 0.0f, 1.0f, 0.0f,	// Front Top Right		- Green	- 1
                0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f,	// Front Bottom Right		- Blue	- 2
                -0.5f,-0.5f, 0.5f, 0.0f, 1.0f, 1.0f,	// Front Bottom Left		- Cyan	- 3
                -0.5f, 0.5f,-0.5f, 1.0f, 0.0f, 1.0f,	// Back Top Left		- Pink	- 4
                0.5f,  0.5f,-0.5f, 1.0f, 1.0f, 0.0f,	// Back Top Right		- Yellow- 5
                0.5f, -0.5f,-0.5f, 0.1f, 0.1f, 0.1f,	// Back Bottom Right		- White - 6
                -0.5f,-0.5f,-0.5f, 1.0f, 1.0f, 1.0f,	// Back Bottom Left		- Gray  - 7
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