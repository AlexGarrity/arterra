#pragma once

#include "PCH.hpp"

#include <immintrin.h>

#include <glm/gtc/noise.hpp>

#include "world/Chunk.hpp"
#include "block/BlockManager.hpp"

namespace arterra {

	class TerrainGenerator {
		public:
			TerrainGenerator();

			void GenerateChunk(Chunk &out, BlockManager &blockManager);
			void GenerateBlock(int x, int y, int z, Block &block);

		private:
	};

}