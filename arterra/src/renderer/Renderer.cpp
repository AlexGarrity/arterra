#include "renderer/Renderer.hpp"

namespace arterra {

	Renderer::Renderer(Camera* camera)
		: _camera { camera }
	{
		// Give GLAD the GLFW extension loader function
		gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress));

		_textureHandle.Load("textures/stone.png");

		_testModel.Create("models/cube.mobj");

		// Load the basic shader and use it
		_shaderManager.LoadShader("shaders/basic.frag", "shaders/basic.vert", "basic");
		_shaderManager.UseShader("basic");

		// Enable depth testing
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_BLEND);

		// Enable face culling
		glFrontFace(GL_CCW);
		glCullFace(GL_BACK);
		glEnable(GL_CULL_FACE);
	}

	void Renderer::Update()
	{
		// Set the camera view projection so the world renders in perspective
		_shaderManager.ActiveProgram().SetUniform("viewProjection", _camera->ViewProjection());
		_shaderManager.ActiveProgram().SetUniform("fragmentColour", {0.2f, 1.0f, 1.0f, 1.0f});
	}

	void Renderer::DrawTestCube()
	{
		_shaderManager.UseShader("basic");
		_shaderManager.ActiveProgram().SetUniform("fragmentTexture", 0);
		_testModel.Bind();
		_textureHandle.Bind();
		glDrawArrays(GL_TRIANGLES, 0, 36);
	}

}