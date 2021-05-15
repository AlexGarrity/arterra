#pragma once

#include "PCH.hpp"

#include "renderer/VertexArray.hpp"
#include "renderer/VertexBuffer.hpp"

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
		explicit Model(const std::string &filepath);

		bool Create(const std::string &filepath);

		void Bind() const;

	private:
		ModelBuffers _buffers;
		size_t _vertexCount;
	};

}