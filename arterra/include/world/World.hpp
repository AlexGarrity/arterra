#pragma once

#include "PCH.hpp"

#include "Chunk.hpp"

namespace arterra {

	using ChunkMap = std::unordered_map<BlockPosition, Chunk, BlockPositionHash>;

	class World {
	public:
		World();

		Chunk* CreateChunk(int x, int y, int z);
		Chunk* CreateChunk(int x, int z);
		Chunk* GetChunk(int x, int y, int z);
		Chunk* GetChunk(int x, int z);

		SubChunk* GetSubChunk(int x, int y, int z);

		Block* GetBlock(int x, int y, int z);

		void Update(float deltaTime);
		std::vector<SubChunk*> &GetModifiedSubChunks();

	private:
		ChunkMap::iterator FindChunk(int x, int z);
		std::vector<SubChunk*> _modifiedSubChunks;

		ChunkMap _chunks;
	};

}