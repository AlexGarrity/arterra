#include "renderer/Renderer.hpp"

namespace arterra {

Renderer::Renderer(Camera* camera)
    : _camera { camera }
{
    // Give GLAD the GLFW extension loader function
    gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress));

    vao.Bind();


    // Assign position buffer.
    vbP.Create(_positions, 3, GL_FLOAT);
    vao.AddBuffer(vbP);

    // Assign colour buffer.
    vbC.Create(_colours, 3, GL_FLOAT);
    vao.AddBuffer(vbC);

    // Load the basic shader and use it
    _shaderManager.LoadShader("shaders/basicColour.frag", "shaders/basicColour.vert", "basic");
    _shaderManager.UseShader("basic");

    // Enable depth testing
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);

    // Enable face culling
    glFrontFace(GL_CW);
    glCullFace(GL_BACK);
    glEnable(GL_CULL_FACE);
}

void Renderer::Update()
{
    // Set the camera view projection so the world renders in perspective
    _shaderManager.ActiveProgram().SetUniform("viewProjection", _camera->ViewProjection());
}

void Renderer::DrawTestCube()
{
    _shaderManager.UseShader("basic");
    vao.Bind();
    glDrawArrays(GL_TRIANGLES, 0, 36);
}

}