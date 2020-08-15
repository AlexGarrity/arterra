#pragma once

#include "PCH.hpp"

#include "world/ChunkPool.hpp"
#include "world/WorldPosition.hpp"

namespace arterra {

	class Chunk;
	class SubChunk;
	class Block;
	class TerrainGenerator;
	class BlockManager;

	using ChunkMap = std::unordered_map<ChunkPosition, Chunk*, ChunkPositionHash>;

	class World {
	public:
		World(TerrainGenerator* generator, BlockManager* blockManager);

		Chunk* CreateChunk(int x, int y, int z);
		Chunk* GetChunk(int x, int y, int z);
		SubChunk* GetSubChunk(int x, int y, int z);

		Chunk* CreateChunkCS(int x, int z);
		Chunk* GetChunkCS(int x, int z);
		SubChunk* GetSubChunkCS(int x, int y, int z);

		void DeleteChunk(int x, int z);
		void DeleteChunkCS(int x, int z);

		Block* GetBlock(int x, int y, int z);

		void Update(float deltaTime, glm::vec3 playerPos);
		void GenerateNewChunks(glm::vec3 playerPosition);
		void DeleteOldChunks(glm::vec3 playerPosition);

		std::vector<SubChunk*>& GetModifiedSubChunks();
		std::vector<WorldPosition>& GetEmptyChunks();

		float_t DistanceToChunk(glm::vec3 pos, ChunkPosition c);
		float_t DistanceToChunk2(glm::vec3 pos, ChunkPosition c);
		bool ChunkInLoadDistance(glm::vec3 pos, ChunkPosition c);

		float_t DistanceToChunk(ChunkPosition a, ChunkPosition c);
		float_t DistanceToChunk2(ChunkPosition a, ChunkPosition c);
		bool ChunkInLoadDistance(ChunkPosition a, ChunkPosition c);

		void ResizeLoadDistance(float_t distance);

	private:
		ChunkMap::iterator FindChunk(int x, int z);
		ChunkMap::iterator FindChunkCS(int x, int z);

		std::vector<SubChunk*> _modifiedSubChunks;
		std::vector<WorldPosition> _emptyChunks;

		ChunkMap _chunks;
		ChunkPool _chunkPool;
		TerrainGenerator* _terrainGenerator;
		BlockManager* _blockManager;

		// Number of chunks to load
		float_t _loadDistance { 10 };
		float_t _loadDistance2 { _loadDistance * _loadDistance };
	};

}