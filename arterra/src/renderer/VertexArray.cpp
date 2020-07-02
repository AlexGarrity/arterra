#include "renderer/VertexArray.hpp"
#include "renderer/VertexBuffer.hpp"

namespace arterra {

	VertexArray::VertexArray()
	{
		// Generates a new vertex array.
		glGenVertexArrays(1, &_glID);
	}

	VertexArray::~VertexArray() { glDeleteVertexArrays(1, &_glID); }

	void VertexArray::AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout)
	{
		// Bind this vertex array if it's not already bound.
		Bind();
		// Bind the vertex buffer it it's not already bound.
		vb.Bind();
		
		// Get all individual defined elements (vector properties) for this buffer layout.
		const auto& elements = layout.GetElements();
		unsigned int offset = 0;
		for (GLuint i = 0; i < elements.size(); i++) {
			// For each element, enable it in the Vertex Attribute Array.
			const auto& element = elements[i];
			glEnableVertexAttribArray(i);
			// Set the attribute pointer to the correct value, e.g.
			//		For a position vector
			//		i = 0 ( position vector is first )
			//		_count = 3 ( vector has 3 coords x,y,z )
			//		_type = GL_FLOAT ( since its a vector )
			// 		_normalised = GL_FALSE ( not needed )
			// 		GetStride() = 6 * sizeof(float) ( there are 3 position floats and 3 colour float )
			//		offset = 0 ( since position is first )
			glVertexAttribPointer(
				i, element._count, element._type, element._normalised, layout.GetStride(), (const void*)offset);
			// Increase the offset by the element total size, e.g.
			//		position is a vec3, so 3 (floats) * sizeof(float)
			offset += element._count * VertexBufferLayout::Element::GetSizeOfType(element._type);
		}
	}

	void VertexArray::Bind() const { glBindVertexArray(_glID); }

	void VertexArray::Unbind() const { glBindVertexArray(0); }

}