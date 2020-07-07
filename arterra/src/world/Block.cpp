#include "world/Block.hpp"
#include "world/SubChunk.hpp"

namespace arterra {

	Block::Block(const Block& other)
		: _model(other._model)
		, _subChunk(other._subChunk)
		, _position(other.GetPositionRaw())
	{
	}

	Block::Block(int posX, int posY, int posZ, CullableModel& model, SubChunk* subChunk)
		: _position(posX, posY, posZ)
		, _model(model)
		, _subChunk(subChunk)
	{
	}

	BlockPosition Block::GetPosition() const
	{
		return BlockPosition { _position._x + _subChunk->_posX, _position._y + _subChunk->_posY,
			_position._z + _subChunk->_posZ };
	}

	BlockPosition Block::GetPositionRaw() const { return _position; }

	CullableModel& Block::GetModel() const { return _model; }

	std::array<bool, 6> Block::GetVisibleFaces() const { return _visibleFaces; }

}