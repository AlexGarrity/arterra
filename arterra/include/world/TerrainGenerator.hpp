#pragma once

#include "PCH.hpp"



namespace arterra {

	class ThreadManager;
	class Chunk;
	class BlockManager;
	class Block;

	class TerrainGenerator {
		using GeneratorJob = std::function<void()>;

	public:
		TerrainGenerator(ThreadManager *threadManager, BlockManager* blockManager);
		~TerrainGenerator();

		/**
		 * @brief Adds a chunk to the generator queue to be generated off the main thread
		 **/
		void AddChunkToGeneratorQueue(Chunk* chunk);

		void MarkChunkAsCompleted(Chunk* c);

		std::vector<Chunk*>& GetCompletedChunks();

		void GenerateChunk(Chunk& out);
		void GenerateBlock(int x, int y, int z, Block& block);

	private:
		uint16_t seedX, seedY, seedZ;

		std::set<Chunk*> _pendingChunks;
		std::vector<Chunk*> _completedChunks;

		ThreadManager* _threadManager;
		BlockManager* _blockManager;
		bool _exiting;
	};

}