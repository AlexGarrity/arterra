#pragma once

#include "PCH.hpp"

#include <glm/glm.hpp>

#include "Chunk.hpp"

namespace arterra {

	using ChunkMap = std::unordered_map<ChunkPosition, Chunk*, ChunkPositionHash>;

	class World {
	public:
		World();

		Chunk* CreateChunk(int x, int y, int z);
		Chunk* GetChunk(int x, int y, int z);
		SubChunk* GetSubChunk(int x, int y, int z);

		Chunk* CreateChunkCS(int x, int z);
		Chunk* GetChunkCS(int x, int z);
		SubChunk* GetSubChunkCS(int x, int y, int z);

		void DeleteChunk(int x, int z);
		void DeleteChunkCS(int x, int z);

		Block* GetBlock(int x, int y, int z);

		void Update(float deltaTime);
		void GenerateNewChunks(glm::vec3 playerPosition);
		void DeleteOldChunks(glm::vec3 playerPosition);

		std::vector<SubChunk*>& GetModifiedSubChunks();
		std::vector<Chunk*>& GetEmptyChunks();

		float_t DistanceToChunk(glm::vec3 pos, ChunkPosition c);
		float_t DistanceToChunk2(glm::vec3 pos, ChunkPosition c);
		bool ChunkInLoadDistance(glm::vec3 pos, ChunkPosition c);

		float_t DistanceToChunk(ChunkPosition a, ChunkPosition c);
		float_t DistanceToChunk2(ChunkPosition a, ChunkPosition c);
		bool ChunkInLoadDistance(ChunkPosition a, ChunkPosition c);

	private:
		ChunkMap::iterator FindChunk(int x, int z);
		ChunkMap::iterator FindChunkCS(int x, int z);

		std::vector<Chunk*> _emptyChunks;
		std::vector<SubChunk*> _modifiedSubChunks;

		ChunkMap _chunks;
		// Number of chunks to load
		float_t _loadDistance { 10 };
		float_t _loadDistance2 { _loadDistance * _loadDistance };
	};

}