#include "world/SubChunk.hpp"

namespace arterra {
	
	SubChunk::SubChunk(int posX, int posY, int posZ, CullableModel& model) 
		: _posX(posX), _posY(posY), _posZ(posZ) {
		
		for (int y = 0; y < 16; y++)
		{
			for (int z = 0; z < 16; z++)
			{
				for (int x = 0; x < 16; x++)
				{
					_blocks[x + (16 * (z + (16 * y)))] = new Block(x, y, z, model, this);
				}
				
			}
			
		}
		
		
		
	}
	
	
	std::array<Block*, 4096>& SubChunk::GetBlocks() {
		return _blocks;
	}
	
	
	
}