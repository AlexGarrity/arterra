#pragma once

#include "PCH.hpp"
#include <glad/glad.h>

namespace arterra {

	class VertexBuffer {

	public:
		VertexBuffer();
		// dtor calls the glDeleteBuffers function.
		~VertexBuffer();

		void Destroy();

		// A vertex buffer contains data used to describe information
		// about vertices, such as position, colour, uv, normals etc.
		// When drawing a vao, you can define multiple vertex buffers
		// to keep the information separate, or supply a single buffer
		// which contains everything. The second option requires specifying
		// data strides and offsets.
		//
		// data = array making up the data.
		// size = total size of the array in bytes.
		void Create(std::vector<float>& data, GLint count, GLenum type);

		// Set this buffer to be the current buffer to work on.
		void Bind() const;
		// Set the current buffer to 0.
		void Unbind() const;

		GLsizei GetSize() const;

		GLint GetCount() const;
		GLenum GetType() const;

		GLint GetVertexCount() const;

	private:
		// The unique opengl id for this buffer.
		GLuint _glID;
		// The number of values for this element, e.g. 3 floats.
		GLint _count;
		// The type of element this buffer contains.
		GLenum _type;

		GLint _vertexCount;
	};

}