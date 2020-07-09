#pragma once

#include "PCH.hpp"

#include "Chunk.hpp"

namespace arterra {

	struct BlockPositionHash {
		size_t operator()(const BlockPosition& pos) const
		{
			return std::hash<int> {}(pos._x) ^ std::hash<int> {}(pos._y) ^ std::hash<int> {}(pos._z);
		}
	};

	using ChunkMap = std::unordered_map<BlockPosition, Chunk, BlockPositionHash>;

	class World {
	public:
		World();

		Chunk* CreateChunk(int x, int y, int z, CullableModel &model);
		Chunk* CreateChunk(int x, int z, CullableModel &model);
		Chunk* GetChunk(int x, int y, int z);
		Chunk* GetChunk(int x, int z);

		SubChunk* GetSubChunk(int x, int y, int z);

		Block* GetBlock(int x, int y, int z);

		void Update(float deltaTime);
		std::vector<SubChunk*> &GetUpdatedChunks();

	private:
		ChunkMap::iterator FindChunk(int x, int z);
		std::vector<SubChunk*> _updatedSubChunks;

		ChunkMap _chunks;
	};

}