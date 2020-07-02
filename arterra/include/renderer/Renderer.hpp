#pragma once
#include "PCH.hpp"

#include <glad/glad.h>

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "model/Model.hpp"
#include "renderer/Camera.hpp"
#include "renderer/ShaderManager.hpp"
#include "renderer/Texture.hpp"

namespace arterra {
class Renderer {
public:
    Renderer(Camera& camera);

    // Generate a new vertex array object
    void GenerateVAO();

    // Update the renderer to account for input
    void Update();

    // Draw a rainbow cube
    void DrawTestCube();

private:
    GLuint _vao;

    ShaderManager _shaderManager;
    Camera* _camera;
    TextureHandle _stoneTexture;
    Model _cubeModel;

    GLint _viewProjectionUniform;
};

}