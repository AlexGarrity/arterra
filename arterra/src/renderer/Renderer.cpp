#include "renderer/Renderer.hpp"

namespace arterra {

    Renderer::Renderer() {
        // Load GL core using GLAD, if it fails then error and return
        if (!gladLoadGL()) {
            Logger::Get().Log("GLAD failed to initialise", Logger::Severity::Fatal);
        }

        // Give GLAD the GLFW extension loader function
        gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress));
    
        GenerateVAO();
        GenerateVBO();
        GenerateEBO();

        _shaderManager.LoadShader("res/shaders/basic.frag", "res/shaders/basic.vert", "basic");
        _shaderManager.UseShader("basic");

        glEnable(GL_DEPTH_TEST);
    }


    void Renderer::GenerateVAO() {
        glGenVertexArrays(1, &_vao);
        glBindVertexArray(_vao);
    }

    void Renderer::GenerateVBO() {
        glGenBuffers(1, &_vbo);
        glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    }

    void Renderer::GenerateEBO() {
        glGenBuffers(1, &_ebo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebo);
    }


    void Renderer::DrawPoints(std::vector<GLfloat> points, std::vector<GLuint> elements) {
        glBindBuffer(GL_ARRAY_BUFFER, _vbo);
        glBufferData(GL_ARRAY_BUFFER, points.size() * sizeof(GLfloat), points.data(), GL_STATIC_DRAW);
    
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, elements.size() * sizeof(GLuint), elements.data(), GL_STATIC_DRAW);

        glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(elements.size()), GL_UNSIGNED_INT, 0);
    }

    void Renderer::DrawTestCube() {
        DrawPoints(_vertices, _elements);
    }

}