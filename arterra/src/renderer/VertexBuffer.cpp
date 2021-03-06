#include "renderer/VertexBuffer.hpp"

namespace arterra {

	size_t GetTypeSize(GLenum type)
	{
		switch (type) {
			case GL_FLOAT:
				return 4;
			case GL_UNSIGNED_SHORT:
				return 2;
		}
		return 0;
	}

	VertexBuffer::VertexBuffer() {}

	VertexBuffer::~VertexBuffer() {  }

	void VertexBuffer::Destroy() {
		glDeleteBuffers(1, &_glID);
	}

	void VertexBuffer::Create(std::vector<float>& data, GLint count, GLenum type)
	{
		// Initialise the variables.
		_count = count;
		_type = type;
		_vertexCount = data.size() / count;
		// Calculate array size for setting buffer data.
		auto typeSize = GetTypeSize(_type);
		// Generate a new buffer for this vertex buffer.
		glGenBuffers(1, &_glID);
		// Bind it as the current buffer.
		glBindBuffer(GL_ARRAY_BUFFER, _glID);
		// Pass in the vertex data into the buffer
		glBufferData(GL_ARRAY_BUFFER, typeSize * data.size(), data.data(), GL_STATIC_DRAW);
	}

	void VertexBuffer::Bind() const { glBindBuffer(GL_ARRAY_BUFFER, _glID); }

	void VertexBuffer::Unbind() const { glBindBuffer(GL_ARRAY_BUFFER, 0); }

	GLsizei VertexBuffer::GetSize() const { return GetTypeSize(_type); }
	
	GLint VertexBuffer::GetCount() const { return _count; }
	
	GLenum VertexBuffer::GetType() const { return _type; }
	
	GLint VertexBuffer::GetVertexCount() const { return _vertexCount; }

}
