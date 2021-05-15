#pragma once

#include <glad/glad.h>

#include "renderer/VertexArray.hpp"
#include "renderer/VertexBuffer.hpp"

namespace arterra {
	
	class GuiElement {

	public:
		GuiElement();
		GuiElement(std::vector<float_t> posVertices, std::vector<float_t> uvVertices);

		void Destroy();
		void GenerateMesh();

		void SetShouldRender(bool shouldRender);
		bool ShouldRender() const;

		void Bind();
		GLuint GetVertexCount();

	private:
		std::vector<float_t> _posVertices;
		std::vector<float_t> _uvVertices;
		VertexArray _vertexArray;

		VertexBuffer _posBuffer;
		VertexBuffer _texBuffer;

		bool _shouldRender = false;
	};
	
}