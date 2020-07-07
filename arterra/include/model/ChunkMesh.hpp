#pragma once

#include "PCH.hpp"

#include "model/CullableModel.hpp"
#include "world/SubChunk.hpp"

namespace arterra {

	class ChunkMesh {
	public:
		ChunkMesh();

		void AddChunk();
		void AddSubChunk(SubChunk& subChunk);
		void AddBlock(Block& block);
		void AddFace(std::vector<float_t> vertices);

		void GenerateMesh();

		void Bind();
		GLuint GetVertexCount();

	private:
		std::vector<float_t> _vertices;
		VertexArray _vertexArray;
		VertexBuffer _vertexBuffer;
	};

}