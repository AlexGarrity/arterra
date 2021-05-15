#pragma once

#include "PCH.hpp"

#include <glm/gtc/noise.hpp>

namespace arterra {

	class Chunk;
	class BlockManager;
	class Block;

	class TerrainGenerator {
		using GeneratorJob = std::function<void()>;

	public:
		TerrainGenerator(BlockManager* blockManager);
		~TerrainGenerator();

		/**
		 * @brief Adds a chunk to the generator queue to be generated off the main thread
		 **/
		void AddChunkToGeneratorQueue(Chunk* chunk);

		Chunk* GetNextChunkToGenerate();
		void PopChunk();

		void SetAwaitShutdown(bool b) { _exiting = b; }
		bool IsAwaitingShutdown() const { return _exiting; }

		void MarkChunkAsCompleted(Chunk* c);

		std::vector<Chunk*>& GetCompletedChunks();

		void CreateChunkGeneratorThread();

		void GenerateChunk(Chunk& out);
		void GenerateBlock(int x, int y, int z, Block& block);

	private:
		uint16_t seedX, seedY, seedZ;

		std::set<Chunk*> _pendingChunks;
		std::vector<Chunk*> _completedChunks;
		std::queue<GeneratorJob> _pendingJobs;
		BlockManager* _blockManager;

		std::thread _chunkGeneratorThread;
		std::mutex _chunkQueueLock;
		std::mutex _completedChunksLock;
		bool _exiting;
	};

}