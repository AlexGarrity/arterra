#include "renderer/Renderer.hpp"

namespace arterra {

	Renderer::Renderer(Camera* camera)
		: _camera { camera }
	{
		// Give GLAD the GLFW extension loader function
		gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress));

		_textureHandle.Load("textures/stone.png");

		// Enable depth testing
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_BLEND);

		// Enable face culling
		glFrontFace(GL_CCW);
		glCullFace(GL_BACK);
		glEnable(GL_CULL_FACE);

		// Enable blending
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}

	void Renderer::Update() {}

	void Renderer::DrawTestCube() { _textureHandle.Bind(); }

}