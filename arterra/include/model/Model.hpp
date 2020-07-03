#pragma once

#include "PCH.hpp"

#include "glad/glad.h"

#include "renderer/VertexArray.hpp"
#include "util/Resource.hpp"

namespace arterra {

	class Model {
		struct ModelBuffers {

			VertexBuffer _positions;
			VertexBuffer _uvs;
			VertexBuffer _normals;

			VertexArray _vao;
		};

	public:
		// Default ctor
		Model() = default;
		// RAII ctor
		Model(std::string filepath);

		bool Create(std::string filepath);

		inline void Bind() const { _buffers._vao.Bind(); }

	private:
		ModelBuffers _buffers;
		size_t _vertexCount;
	};

}