#include "renderer/VertexArray.hpp"

#include "renderer/VertexBuffer.hpp"

namespace arterra {

	VertexArray::VertexArray()
		: _vertexCount(0)
	{
		// Generates a new vertex array.
		glGenVertexArrays(1, &_glID);
	}

	VertexArray::VertexArray(VertexArray& other)
	{
		_glID = other._glID;
		_lastBoundAttribute = other._lastBoundAttribute;
		_vertexCount = other._vertexCount;
	}

	VertexArray::~VertexArray() {  }

	void VertexArray::Destroy() {
		glDeleteVertexArrays(1, &_glID);
	}

	void VertexArray::AddBuffer(const VertexBuffer& vb)
	{
		// Bind this vertex array if it's not already bound.
		Bind();
		// Bind the vertex buffer it it's not already bound.
		vb.Bind();

		if (!_lastBoundAttribute)
			_vertexCount = vb.GetVertexCount();

		// Set the attribute pointer to the correct value, e.g.
		//		For a position vector
		//		i = 0 ( position vector is first )
		//		GetCount = 3 ( vector has 3 coords x,y,z )
		//		_type = GL_FLOAT ( since its a vector )
		// 		_normalised = GL_FALSE ( not needed )
		// 		GetStride() = 6 * sizeof(float) ( there are 3 position floats and 3 colour float)
		//		offset = 0 ( since position is first )
		glVertexAttribPointer(
			_lastBoundAttribute, vb.GetCount(), vb.GetType(), GL_FALSE, vb.GetCount() * vb.GetSize(), 0);

		glEnableVertexAttribArray(_lastBoundAttribute);

		_lastBoundAttribute++;
	}

	void VertexArray::Bind() const { glBindVertexArray(_glID); }

	void VertexArray::Unbind() const { glBindVertexArray(0); }

	GLuint VertexArray::GetVertexCount() const { return _vertexCount; }
}