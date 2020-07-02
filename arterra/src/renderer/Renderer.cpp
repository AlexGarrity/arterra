#include "renderer/Renderer.hpp"

namespace arterra {

    Renderer::Renderer(Camera &camera) : _camera{&camera} {
        // Load GL core using GLAD, if it fails then error and return
        if (!gladLoadGL()) {
            Logger::Get().Log(Logger::Fatal, "GLAD failed to initialise");
        }

        // Give GLAD the GLFW extension loader function
        gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress));
    
        // Generate buffers
        GenerateVAO();

        // Load the basic shader and use it
        _shaderManager.LoadShader("shaders/basic.frag", "shaders/basic.vert", "basic");

        _stoneTexture.Load("textures/stone.png");
        _cubeModel.Create("models/torus.mobj");

        // Enable depth testing
        glEnable(GL_DEPTH_TEST);
    }


    void Renderer::GenerateVAO() {
        // Generate the vertex array and bind it
        glGenVertexArrays(1, &_vao);
        glBindVertexArray(_vao);
    }

    void Renderer::Update() {
        // Set the camera view projection so the world renders in perspective
    }

    void Renderer::DrawTestCube() {
        glBindTexture(GL_TEXTURE_2D, _stoneTexture.Handle());
        _shaderManager.UseShader("basic");
        _shaderManager.ActiveProgram().SetUniform("viewProjection", _camera->ViewProjection());
        _shaderManager.ActiveProgram().SetUniform("fragmentColour", {1.0f});
        _cubeModel.Render();
    }

}