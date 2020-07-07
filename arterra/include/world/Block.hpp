#pragma once

#include "PCH.hpp"
#include "model/CullableModel.hpp"

namespace arterra {
	
	struct BlockPosition{
		int _x;
		int _y;
		int _z;
		
		BlockPosition(int x, int y, int z)
			: _x(x), _y(y), _z(z) {
				
			}
	};
	
	
	class SubChunk;
	
	class Block {
		
		
		public:
			int _posX;
			int _posY;
			int _posZ;
			
			Block(const Block& block);
			Block(int posX, int posY, int posZ, CullableModel& model, SubChunk* subChunk);	
			
			BlockPosition GetPosition();
		
			CullableModel& _model;
			
			SubChunk* _subChunk;
			
			
	};
	
}