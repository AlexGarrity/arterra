#pragma once

#include "PCH.hpp"

#include "glad/glad.h"

#include "util/Resource.hpp"

namespace arterra {

	class Model {
		struct ModelBuffers {
			GLuint _vertexBuffer = 0;
			GLuint _uvBuffer = 0;
			GLuint _normalBuffer = 0;

			~ModelBuffers()
			{
				glDeleteBuffers(1, &_vertexBuffer);
				glDeleteBuffers(1, &_uvBuffer);
				glDeleteBuffers(1, &_normalBuffer);
			}
		};

	public:
		// Default ctor
		Model();
		// RAII ctor
		Model(std::string filepath);
		Model::~Model();

		bool Create(std::string filepath);
		void Render();

	private:
		ModelBuffers _buffers;
		size_t _vertexCount;
	};

}