#pragma once

#include "PCH.hpp"
#include "world/Block.hpp"

namespace arterra {
	
	class SubChunk {
		
		public:
			int _posX;
			int _posY;
			int _posZ;
		
			// Contains 16 x 16 x 16 blocks.
			std::array<Block*, 4096> _blocks;
			
			SubChunk(int posX, int posY, int posZ, CullableModel& model);
			
			std::array<Block*, 4096>& GetBlocks();
			
		private:
		
	};
	
}