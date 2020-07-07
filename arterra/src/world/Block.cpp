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
		auto scPosition = _subChunk->GetPosition();
		return BlockPosition { _position._x + scPosition._x, _position._y + scPosition._y,
			_position._z + scPosition._z };
	}

	BlockPosition Block::GetPositionRaw() const { return _position; }

	CullableModel& Block::GetModel() const { return _model; }

	std::array<bool, 6> Block::GetVisibleFaces() const { return _visibleFaces; }

	void Block::Update(size_t width)
	{
		UpdateNeighbours();
		UpdateVisiblity();

		if (width != 0) {
			for (auto n : _neighbours) {
				if (!n)
					continue;
				n->Update(width - 1);
			}
		}
	}

	void Block::UpdateVisiblity()
	{
		bool visible = false;
		for (auto i = 0; i < 6; ++i) {
			auto blockExists = (_neighbours[i] != nullptr);
			_visibleFaces[i] = (!blockExists);
			if (!blockExists)
				visible = true;
		}
		_visible = visible;
	}

	void Block::UpdateNeighbours()
	{
		_neighbours[0] = _subChunk->GetBlock(_position._x + 1, _position._y, _position._z);
		_neighbours[1] = _subChunk->GetBlock(_position._x - 1, _position._y, _position._z);
		_neighbours[2] = _subChunk->GetBlock(_position._x, _position._y + 1, _position._z);
		_neighbours[3] = _subChunk->GetBlock(_position._x, _position._y - 1, _position._z);
		_neighbours[4] = _subChunk->GetBlock(_position._x, _position._y, _position._z + 1);
		_neighbours[5] = _subChunk->GetBlock(_position._x, _position._y, _position._z - 1);
	}

}