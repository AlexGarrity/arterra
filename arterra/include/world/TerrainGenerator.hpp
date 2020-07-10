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
			OpenSimplex2D _generator2D;
			OpenSimplex3D _generator3D;
	};

}