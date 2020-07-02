#include "renderer/VertexBuffer.hpp"

namespace arterra {

	VertexBuffer::VertexBuffer() {}

	VertexBuffer::~VertexBuffer() { glDeleteBuffers(1, &_glID); }

	void VertexBuffer::Create(const void* data, unsigned int size)
	{
		// Generate a new buffer for this vertex buffer.
		glGenBuffers(1, &_glID);
		// Bind it as the current buffer.
		glBindBuffer(GL_ARRAY_BUFFER, _glID);
		// Pass in the vertex data into the buffer
		glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
	}

	void VertexBuffer::Bind() const { glBindBuffer(GL_ARRAY_BUFFER, _glID); }

	void VertexBuffer::Unbind() const { glBindBuffer(GL_ARRAY_BUFFER, 0); }

}