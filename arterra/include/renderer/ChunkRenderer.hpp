#pragma once

#include "PCH.hpp"

#include "model/ChunkMesh.hpp"
#include "world/SubChunk.hpp"
#include "world/Chunk.hpp"

namespace arterra {

	class ChunkRenderer {
		public:
			ChunkRenderer();

			void AddChunk(Chunk &chunk);
			void AddSubChunk(SubChunk &subChunk);
			void AddMesh(ChunkMesh mesh);

			void Render();

		private:
			std::vector<ChunkMesh> _renderables;
	};

}