#include "world/Block.hpp"

#include "world/Chunk.hpp"
#include "world/SubChunk.hpp"
#include "world/World.hpp"

namespace arterra {

	Block::Block(const Block& other)
		: _subChunk(other._subChunk)
		, _position(other.GetPositionRaw())
		, _blockData(other._blockData)
		, _world(other._world)
	{
	}

	Block::Block(int posX, int posY, int posZ, SubChunk* subChunk, BlockData& blockData)
		: _position(posX, posY, posZ)
		, _subChunk(subChunk)
		, _blockData(blockData)
	{
		_world = _subChunk->GetChunk()->GetWorld();
	}

	void Block::operator=(const Block& other)
	{
		_position = other._position;
		_subChunk = other._subChunk;
		_blockData = other._blockData;
		_world = other._world;
	}

	WorldPosition Block::GetPosition() const
	{
		auto scPosition = _subChunk->GetPosition();
		return WorldPosition { _position._x + scPosition._x, _position._y + scPosition._y,
			_position._z + scPosition._z };
	}

	WorldPosition Block::GetPositionRaw() const { return { _position._x, _position._y, _position._z }; }

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

	void Block::UpdateVisiblity(std::array<Block*, 6>& neighbours)
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
		if (_position._y == 15)
			printf("");
		auto pos = GetPosition();
		auto b1 = _world->GetBlock(pos._x + 1, pos._y, pos._z);
		auto b2 = _world->GetBlock(pos._x - 1, pos._y, pos._z);
		auto b3 = _world->GetBlock(pos._x, pos._y + 1, pos._z);
		auto b4 = _world->GetBlock(pos._x, pos._y - 1, pos._z);
		auto b5 = _world->GetBlock(pos._x, pos._y, pos._z + 1);
		auto b6 = _world->GetBlock(pos._x, pos._y, pos._z - 1);
		return { b1, b2, b3, b4, b5, b6 };
	}

	void Block::SetParent(SubChunk* subChunk)
	{
		_subChunk = subChunk;
		_world = _subChunk->GetChunk()->GetWorld();
	}

}