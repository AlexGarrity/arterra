#pragma once

#include "PCH.hpp"

#include "model/ChunkMesh.hpp"
#include "world/Chunk.hpp"
#include "world/SubChunk.hpp"

namespace arterra {

	class ChunkRenderer {
	public:
		ChunkRenderer();
		~ChunkRenderer();

		void AddChunk(Chunk* chunk);
		void AddChunk(Chunk& chunk);
		void AddSubChunk(SubChunk* subChunk);
		void AddSubChunk(SubChunk& subChunk);
		void AddMesh(ChunkMesh mesh);

		void UpdateSubChunks(std::vector<SubChunk*>& subChunks);
		ChunkMesh* GetChunkMesh(BlockPosition position);
		void DeleteMesh(BlockPosition position);

		void Render();

		std::vector<ChunkMesh>& GetRenderables();

	private:
		std::vector<ChunkMesh> _renderables;
	};

}