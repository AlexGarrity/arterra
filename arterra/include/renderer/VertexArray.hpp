#pragma once

#include "PCH.hpp"
#include <glad/glad.h>

#include "renderer/VertexBuffer.hpp"

namespace arterra {

	class VertexArray {

	public:
		// ctor creates a new vertex array.
		VertexArray();
		// copy ctor
		VertexArray(VertexArray& other);
		// dtor calls the glDeleteVertexArray function.
		~VertexArray();

		void AddBuffer(const VertexBuffer& vb);

		// Sets this vertex array as the current vertex array to work on.
		void Bind() const;
		// Sets the current vertex array to 0.
		void Unbind() const;

		void Destroy();

		GLuint GetVertexCount();

	private:
		// The unique opengl id for this vertex array.
		GLuint _glID;
		// Stores the last bound vertex attribute (number).
		// This allows to bind multiple vertex buffers to this one vertex array
		// and storing vertices data separately.
		GLuint _lastBoundAttribute = 0;

		GLuint _vertexCount;
	};

}