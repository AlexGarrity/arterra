#include "renderer/VertexArray.hpp"

namespace arterra {

	VertexArray::VertexArray()
	{
		// Generates a new vertex array.
		glGenVertexArrays(1, &_glID);
	}

	VertexArray::~VertexArray() { glDeleteVertexArrays(1, &_glID); }

	void VertexArray::AddBuffer(const VertexBuffer& vb)
	{
		// Bind this vertex array if it's not already bound.
		Bind();
		// Bind the vertex buffer it it's not already bound.
		vb.Bind();

		glEnableVertexAttribArray(_lastBoundAttribute);
		// Set the attribute pointer to the correct value, e.g.
		//		For a position vector
		//		i = 0 ( position vector is first )
		//		GetCount = 3 ( vector has 3 coords x,y,z )
		//		_type = GL_FLOAT ( since its a vector )
		// 		_normalised = GL_FALSE ( not needed )
		// 		GetStride() = 6 * sizeof(float_t) ( there are 3 position floats and 3 colour float_t)
		//		offset = 0 ( since position is first )
		glVertexAttribPointer(
			_lastBoundAttribute, vb.GetCount(), vb.GetType(), GL_FALSE, vb.GetCount() * vb.GetSize(), 0);

		_lastBoundAttribute++;
	}

	void VertexArray::Bind() const { glBindVertexArray(_glID); }

	void VertexArray::Unbind() const { glBindVertexArray(0); }

}