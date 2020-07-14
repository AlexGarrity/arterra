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
		void AddFace(std::vector<float_t> posVertices, std::vector<float_t> texVertices);

		void GenerateMesh();
		void SetShouldRender(bool shouldRender) { _shouldRender = shouldRender; }
		bool ShouldRender() const { return _shouldRender; }

		void Bind();
		GLuint GetVertexCount();
		WorldPosition GetPosition();

	private:
		std::vector<float_t> _posVertices;
		std::vector<float_t> _uvVertices;
		VertexArray _vertexArray;

		VertexBuffer _posBuffer;
		VertexBuffer _texBuffer;

		WorldPosition _position;

		bool _shouldRender = true;
	};

}