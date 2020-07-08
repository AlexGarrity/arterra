#pragma once

#include "PCH.hpp"

#include "world/Block.hpp"
#include "world/SubChunk.hpp"

namespace arterra {

	class Chunk {
		public:
			Chunk(int posX, int posY, int posZ, CullableModel &model);

			SubChunk *GetSubChunk(int x, int y, int z);
			std::vector<SubChunk> &GetSubChunks();

			BlockPosition GetPosition();

		private:
			std::vector<SubChunk> _subChunks;

			BlockPosition _position;
	};

}