#pragma once

#include "PCH.hpp"

#include <glm/gtc/noise.hpp>

namespace arterra {

	class Chunk;
	class BlockManager;
	class Block;

	class TerrainGenerator {
		public:
			TerrainGenerator();

			void GenerateChunk(Chunk &out, BlockManager &blockManager);
			void GenerateBlock(int x, int y, int z, Block &block);

		private:
			uint16_t seedX, seedY, seedZ;
	};

}