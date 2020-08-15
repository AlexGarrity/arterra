#pragma once

#include "PCH.hpp"

#include "model/CullableModel.hpp"
#include "world/SubChunk.hpp"

namespace arterra {

	class SubChunkMesh {
	public:
		SubChunkMesh();
		SubChunkMesh(SubChunk& subChunk);
		SubChunkMesh(const SubChunkMesh& other);
		~SubChunkMesh();

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
		bool _shouldRender = true;

		VertexArray _vertexArray;

		VertexBuffer _posBuffer;
		VertexBuffer _texBuffer;

		WorldPosition _position;

		std::vector<float_t> _posVertices;
		std::vector<float_t> _uvVertices;
	};

}