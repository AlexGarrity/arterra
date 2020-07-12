#include "world/SubChunk.hpp"

#include "world/Chunk.hpp"

namespace arterra {

	size_t ResolveArrayPosition(int x, int y, int z)
	{
		if (x > SubChunk::SIZE_X - 1 || y > SubChunk::SIZE_Y - 1 || z > SubChunk::SIZE_Z - 1)
			return -1;
		if (x < 0 || y < 0 || z < 0)
			return -1;
		return x + (SubChunk::SIZE_X * (z + (SubChunk::SIZE_Z * y)));
	}

	SubChunk::SubChunk(int posX, int posY, int posZ, Chunk* parent)
		: _position { posX, posY, posZ }
		, _chunk { parent }
	{
		_blocks.fill(nullptr);
	}

	SubChunk::SubChunk(const SubChunk& other)
	{
		_position = other._position;
		_chunk = other._chunk;
		_blocks = other._blocks;
		for (auto& block : _blocks) {
			if (block)
				block->SetParent(this);
		}
	}

	void SubChunk::SetParent(Chunk* chunk) { _chunk = chunk; }

	std::array<Block*, SubChunk::SIZE>& SubChunk::GetBlocks() { return _blocks; }

	Block* SubChunk::GetBlock(int x, int y, int z) const
	{
		auto pos = ResolveArrayPosition(x, y, z);
		if (pos == -1)
			return nullptr;
		return _blocks[pos];
	}

	void SubChunk::SetBlock(int x, int y, int z, BlockData &data)
	{
		auto pos = ResolveArrayPosition(x, y, z);
		if (_blocks[pos])
			delete _blocks[pos];
		_blocks[pos] = new Block(x, y, z, this, data);
		_updated = true;
	}

	void SubChunk::DeleteBlock(int x, int y, int z)
	{
		auto pos = ResolveArrayPosition(x, y, z);
		delete _blocks[pos];
		_updated = true;
	}

	BlockPosition SubChunk::GetPosition()
	{
		auto cp = _chunk->GetPosition();
		return { (_position._x * static_cast<int>(SIZE_X)) + cp._x,
			(_position._y * static_cast<int>(SIZE_Y)) + cp._y, (_position._z * static_cast<int>(SIZE_Z)) + cp._z };
	}

	bool SubChunk::Update(float deltaTime)
	{
		if (_updated) {
			_updated = false;
			return true;
		}
		return false;
	}
}