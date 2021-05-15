#include "renderer/Renderer.hpp"

#include <SFML/OpenGL.hpp>

namespace arterra {

	Renderer::Renderer()
	{
		// Enable depth testing
		glEnable(GL_BLEND);
		glEnable(GL_DEPTH_TEST);

		// Enable face culling
		glFrontFace(GL_CCW);
		glCullFace(GL_BACK);
		glEnable(GL_CULL_FACE);

		// Enable blending
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}

	void Renderer::Update() const {}

	void Renderer::DrawTriangles(size_t vertexCount) const {
		glDrawArrays(GL_TRIANGLES, 0, vertexCount);
	}

}