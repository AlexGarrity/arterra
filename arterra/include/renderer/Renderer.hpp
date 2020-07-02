#pragma once
#include "PCH.hpp"

#include <glad/glad.h>

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "renderer/ShaderManager.hpp"
#include "renderer/Camera.hpp"
#include "renderer/Texture.hpp"
#include "model/Model.hpp"

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
            GLuint _vao;

            ShaderManager _shaderManager;
            Camera *_camera;
            TextureHandle _stoneTexture;
            Model _cubeModel;

            GLint _viewProjectionUniform;
    };


}