#include "world/Block.hpp"
#include "world/Chunk.hpp"
#include "world/SubChunk.hpp"
#include "world/World.hpp"

namespace arterra {

	Block::Block(const Block& other)
		: _subChunk(other._subChunk)
		, _position(other.GetPositionRaw())
		, _blockData(other._blockData)
	{
	}

	Block::Block(int posX, int posY, int posZ, SubChunk* subChunk, BlockData& blockData)
		: _position(posX, posY, posZ)
		, _subChunk(subChunk)
		, _blockData(blockData)
	{
	}

	void Block::operator=(const Block& other)
	{
		_position = other._position;
		_subChunk = other._subChunk;
		_blockData = other._blockData;
	}

	BlockPosition Block::GetPosition() const
	{
		auto scPosition = _subChunk->GetPosition();
		return BlockPosition { _position._x + scPosition._x, _position._y + scPosition._y,
			_position._z + scPosition._z };
	}

	BlockPosition Block::GetPositionRaw() const { return { _position._x, _position._y, _position._z }; }

	BlockData& Block::GetData() const { return _blockData; }

	std::array<bool, 6> Block::GetVisibleFaces() const { return _visibleFaces; }

	void Block::Update(size_t width)
	{
		auto neighbours = GetNeighbours();
		UpdateVisiblity(neighbours);

		if (width != 0) {
			for (auto n : neighbours) {
				if (!n)
					continue;
				n->Update(width - 1);
			}
		}
	}

	void Block::UpdateVisiblity(std::array<Block*, 6> &neighbours)
	{
		_visible = false;
		for (auto i = 0; i < 6; ++i) {
			auto blockExists = (neighbours[i]);
			_visibleFaces[i] = (!blockExists);
			if (!blockExists)
				_visible = true;
		}
	}

	std::array<Block*, 6> Block::GetNeighbours()
	{
		return { _subChunk->GetBlock(_position._x + 1, _position._y, _position._z),
			_subChunk->GetBlock(_position._x - 1, _position._y, _position._z),
			_subChunk->GetBlock(_position._x, _position._y + 1, _position._z),
			_subChunk->GetBlock(_position._x, _position._y - 1, _position._z),
			_subChunk->GetBlock(_position._x, _position._y, _position._z + 1),
			_subChunk->GetBlock(_position._x, _position._y, _position._z - 1) };
	}

	void Block::SetParent(SubChunk* subChunk) { _subChunk = subChunk; }

}