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
        GenerateVBO();
        GenerateEBO();

        // Load the basic shader and use it
        _shaderManager.LoadShader("shaders/basic.frag", "shaders/basic.vert", "basic");
        _shaderManager.UseShader("basic");

        _stoneTexture.Load("textures/stone.png");

        // Enable depth testing
        glEnable(GL_DEPTH_TEST);
    }


    void Renderer::GenerateVAO() {
        // Generate the vertex array and bind it
        glGenVertexArrays(1, &_vao);
        glBindVertexArray(_vao);
    }

    void Renderer::GenerateVBO() {
        // Generate the vertex buffer and bind it
        glGenBuffers(1, &_vbo);
        glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    }

    void Renderer::GenerateEBO() {
        // Generate the element buffer and bind it
        glGenBuffers(1, &_ebo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebo);
    }

    void Renderer::Update() {
        // Set the camera view projection so the world renders in perspective
        _shaderManager.ActiveProgram().SetUniform("viewProjection", _camera->ViewProjection());
        _shaderManager.ActiveProgram().SetUniform("fragmentColour", {1.0f});
    }

    void Renderer::DrawPoints(std::vector<GLfloat> &points, std::vector<GLuint> &elements) {
        glBindTexture(GL_TEXTURE_2D, _stoneTexture.Handle());

        // Bind the vertex buffer so we can buffer data to it
        glBindBuffer(GL_ARRAY_BUFFER, _vbo);
        // Buffer the element data
        glBufferData(GL_ARRAY_BUFFER, points.size() * sizeof(GLfloat), points.data(), GL_STATIC_DRAW);
    
        // Bind the element buffer so we can buffer data to it
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebo);
        // Buffer the element data
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, elements.size() * sizeof(GLuint), elements.data(), GL_STATIC_DRAW);

        // Draw the points using the provided elements as triangles
        glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(elements.size()), GL_UNSIGNED_INT, 0);
    }

    void Renderer::DrawTestCube() {
        // Draw a rainbow cube
        DrawPoints(_vertices, _elements);
    }

}