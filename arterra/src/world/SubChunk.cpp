#include "world/SubChunk.hpp"

#include "world/Chunk.hpp"

namespace arterra {

	size_t ResolveArrayPosition(int x, int y, int z)
	{
		if (x > 15 || y > 15 || z > 15)
			return -1;
		if (x < 0 || y < 0 || z < 0)
			return -1;
		return x + (16 * (z + (16 * y)));
	}

	SubChunk::SubChunk(int posX, int posY, int posZ, CullableModel& model, Chunk *parent)
		: _position { posX, posY, posZ }, _chunk {parent}
	{

		_blocks.fill(nullptr);

		for (int y = 0; y < 16; y++) {
			for (int z = 0; z < 16; z++) {
				for (int x = 0; x < 16; x++) {
					auto pos = ResolveArrayPosition(x, y, z);
					_blocks[pos] = new Block(x, y, z, model, this);
				}
			}
		}
		for (auto block : _blocks) {
			if (block)
				block->Update(0);
		}
	}

	SubChunk::SubChunk(const SubChunk &other) {
		_position = other._position;
		_chunk = other._chunk;
		_blocks = other._blocks;
		for (auto block : _blocks) {
			if (block)
				block->SetParent(this);
		}
	}

	void SubChunk::SetParent(Chunk *chunk) {
		_chunk = chunk;
	}

	std::array<Block*, 4096>& SubChunk::GetBlocks() { return _blocks; }

	Block* SubChunk::GetBlock(int x, int y, int z) const
	{
		auto pos = ResolveArrayPosition(x, y, z);
		if (pos == -1)
			return nullptr;
		return _blocks[pos];
	}

	void SubChunk::SetBlock(int x, int y, int z, Block &b) {
		auto pos = ResolveArrayPosition(x, y, z);
		if (!_blocks[pos]) _blocks[pos] = new Block(b);
		else *_blocks[pos] = std::move(b);
		_updated = true;
	} 

	void SubChunk::DeleteBlock(int x, int y, int z) {
		auto pos = ResolveArrayPosition(x, y, z);
		delete _blocks[pos];
		_updated = true;
	}

	BlockPosition SubChunk::GetPosition()
	{
		auto cp = _chunk->GetPosition();
		return { (_position._x * 16) + cp._x, (_position._y * 16) + cp._y, (_position._z * 16) + cp._z };
	}

	bool SubChunk::Update(float deltaTime) {
		if (_updated) {
			_updated = false;
			return true;
		}
		return false;
	}
}