#pragma once

#include "PCH.hpp"

#include "renderer/VertexArray.hpp"
#include "renderer/VertexBuffer.hpp"
#include "world/WorldPosition.hpp"

namespace arterra {

	class SubChunk;
	class Block;
	struct WorldPosition;
	
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
		void AddFace(std::vector<float> &posVertices, std::vector<float> &texVertices);

		void GenerateMesh();
		void SetShouldRender(bool shouldRender);
		bool ShouldRender() const;

		void Bind();
		GLuint GetVertexCount() const;
		const WorldPosition& GetPosition() const;

	private:
		bool _shouldRender = true;

		VertexArray _vertexArray;

		VertexBuffer _posBuffer;
		VertexBuffer _texBuffer;

		WorldPosition _position;

		std::vector<float> _posVertices;
		std::vector<float> _uvVertices;

		std::vector<float> _pv;
		std::vector<float> _tv;

		static const size_t ESTIMATED_POS_SIZE = 14000;
		static const size_t ESTIMATED_UV_SIZE = 10000;
	};

}