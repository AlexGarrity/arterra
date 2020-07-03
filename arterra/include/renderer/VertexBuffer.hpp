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
		void Create(const void* data, uint64_t size);

		// Set this buffer to be the current buffer to work on.
		void Bind() const;
		// Set the current buffer to 0.
		void Unbind() const;

	private:
		// The unique opengl id for this buffer.
		GLuint _glID;
	};

	class VertexBufferLayout {

	public:
		struct Element {

			// Number of values this element has e.g. 3 floats.
			GLint _count;
			// Type of element e.g. float_t.
			GLenum _type;
			unsigned char _normalised;

			// Return the correct size of the type.
			static size_t GetSizeOfType(GLenum type)
			{
				switch (type) {
					case GL_FLOAT:
						return 4;
					case GL_UNSIGNED_INT:
						return 4;
				}
				return 0;
			}
		};
		

		VertexBufferLayout()
			: _stride(0)
		{
		}

		template <typename T> void Push(GLint count) { static_assert(false); }

		// Add an element declaration to this buffer layout.
		template <> void Push<float_t>(GLint count)
		{
			_elements.push_back({ count, GL_FLOAT, GL_FALSE });
			_stride += VertexBufferLayout::Element::GetSizeOfType(GL_FLOAT) * count;
		}

		// Add an element declaration to this buffer layout.
		template <> void Push<GLuint>(GLint count)
		{
			_elements.push_back({ count, GL_UNSIGNED_INT, GL_FALSE });
			_stride += VertexBufferLayout::Element::GetSizeOfType(GL_UNSIGNED_INT) * count;
		}

		// Get the "stride" - the length in bytes between the definitions
		// of consecutive vectors, i.e. the length of (position, uv, normals...)
		// for a vector.
		inline GLsizei GetStride() const { return static_cast<GLsizei>(_stride); }

		// Return all the elements (such as position, uv, normals)
		// that this buffer layout contains.
		inline const std::vector<VertexBufferLayout::Element> GetElements() const { return _elements; }

	private:
		// All the elements (such as position, uv, normals) in this buffer layout.
		std::vector<VertexBufferLayout::Element> _elements;
		size_t _stride;
	};

}