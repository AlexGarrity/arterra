#pragma once

#include "PCH.hpp"

#include "util/OpenSimplex.hpp"
#include "world/Chunk.hpp"

namespace arterra {

	class TerrainGenerator {
		public:
			TerrainGenerator();

			void GenerateChunk(Chunk &out, Block &block);

		private:
			OpenSimplex _generator;
	};

}