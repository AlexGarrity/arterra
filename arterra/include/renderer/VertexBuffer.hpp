#pragma once

#include "PCH.hpp"
#include <glad/glad.h>

namespace arterra {

	class VertexBuffer {

	public:
		VertexBuffer();
		// dtor calls the glDeleteBuffers function.
		~VertexBuffer();

		// A vertex buffer contains data used to describe information
		// about vertices, such as position, colour, uv, normals etc.
		// When drawing a vao, you can define multiple vertex buffers
		// to keep the information separate, or supply a single buffer
		// which contains everything. The second option requires specifying
		// data strides and offsets.
		//
		// data = array making up the data.
		// size = total size of the array in bytes.
		void Create(std::vector<float_t>& data, GLint count, GLenum type);

		// Set this buffer to be the current buffer to work on.
		void Bind() const;
		// Set the current buffer to 0.
		void Unbind() const;

		GLsizei GetSize() const;

		inline GLint GetCount() const { return _count; }
		inline GLenum GetType() const { return _type; }

		inline GLint GetVertexCount() const { return _vertexCount; }

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