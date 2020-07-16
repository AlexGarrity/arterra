#include "renderer/Renderer.hpp"

namespace arterra {

	Renderer::Renderer()
	{
		// Enable depth testing
		glEnable(GL_BLEND);

		// Enable face culling
		glFrontFace(GL_CCW);
		glCullFace(GL_BACK);
		glEnable(GL_CULL_FACE);

		// Enable blending
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}

	void Renderer::Update() {}

	void Renderer::DrawTriangles(size_t vertexCount) {
		glDrawArrays(GL_TRIANGLES, 0, vertexCount);
	}

}