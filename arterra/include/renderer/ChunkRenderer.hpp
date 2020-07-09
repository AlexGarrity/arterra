#pragma once

#include "PCH.hpp"

#include "model/ChunkMesh.hpp"
#include "world/SubChunk.hpp"
#include "world/Chunk.hpp"

namespace arterra {

	class ChunkRenderer {
		public:
			ChunkRenderer();
			~ChunkRenderer();

			void AddChunk(Chunk *chunk);
			void AddChunk(Chunk &chunk);
			void AddSubChunk(SubChunk &subChunk);
			void AddMesh(ChunkMesh mesh);

			void Render();

			std::vector<ChunkMesh> &GetRenderables();

		private:
			std::vector<ChunkMesh> _renderables;
	};

}