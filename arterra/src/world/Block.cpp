#include "world/Block.hpp"
#include "world/SubChunk.hpp"

namespace arterra {
	
	Block::Block(const Block& block) 
		: _model(block._model), _subChunk(block._subChunk) {
		_posX = block._posX;
		_posY = block._posY;
		_posZ = block._posZ;
	}
	
	Block::Block(int posX, int posY, int posZ, CullableModel& model, SubChunk* subChunk) 
		: _posX(posX), _posY(posY), _posZ(posZ), _model(model), _subChunk(subChunk) {
		
	}
	
	BlockPosition Block::GetPosition() {
		return BlockPosition { _posX + _subChunk->_posX, _posY + _subChunk->_posY, _posZ + _subChunk->_posZ };
	}
	
}