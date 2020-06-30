#include "renderer/Renderer.hpp"

namespace arterra {

    Renderer::Renderer(Camera &camera) : _camera{&camera} {
        // Load GL core using GLAD, if it fails then error and return
        if (!gladLoadGL()) {
            Logger::Get().Log("GLAD failed to initialise", Logger::Severity::Fatal);
        }

        // Give GLAD the GLFW extension loader function
        gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress));
    
        // Generate buffers
        GenerateVAO();
        GenerateVBO();
        GenerateEBO();

        // Load the basic shader and use it
        _shaderManager.LoadShader("res/shaders/basic.frag", "res/shaders/basic.vert", "basic");
        _shaderManager.UseShader("basic");

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
    }

    void Renderer::DrawPoints(std::vector<GLfloat> &points, std::vector<GLuint> &elements) {
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