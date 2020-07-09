#pragma once

#include "PCH.hpp"

#include "model/CullableModel.hpp"
#include "world/SubChunk.hpp"

namespace arterra {

	class ChunkMesh {
	public:
		ChunkMesh();
		ChunkMesh(SubChunk& subChunk);
		ChunkMesh(const ChunkMesh& other);
		~ChunkMesh();

		void Destroy();

		void AddChunk();
		void AddSubChunk(SubChunk& subChunk);
		void AddBlock(Block& block);
		void AddFace(std::vector<float_t> vertices);

		void GenerateMesh();

		void Bind();
		GLuint GetVertexCount();
		BlockPosition GetPosition();

	private:
		std::vector<float_t> _vertices;
		VertexArray _vertexArray;
		VertexBuffer _vertexBuffer;

		BlockPosition _position;
	};

}