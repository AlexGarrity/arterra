#pragma once
#include "PCH.hpp"

namespace arterra {
	class Renderer {
	public:
		Renderer();

		// Update the renderer to account for input
		void Update() const;

		// Draw a triangle array
		void DrawTriangles(size_t vertexCount) const;

	private:
	};

}