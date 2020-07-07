#include "world/SubChunk.hpp"

namespace arterra {

	size_t ResolveArrayPosition(int x, int y, int z)
	{
		if (x > 15 || y > 15 || z > 15)
			return -1;
		if (x < 0 || y < 0 || z < 0)
			return -1;
		return x + (16 * (z + (16 * y)));
	}

	SubChunk::SubChunk(int posX, int posY, int posZ, CullableModel& model)
		: _posX(posX)
		, _posY(posY)
		, _posZ(posZ)
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
			if (block) block->Update(0);
		}
	}

	std::array<Block*, 4096>& SubChunk::GetBlocks() { return _blocks; }

	Block* SubChunk::GetBlock(int x, int y, int z)
	{
		auto pos = ResolveArrayPosition(x, y, z);
		if (pos == -1)
			return nullptr;
		return _blocks[pos];
	}

	BlockPosition SubChunk::GetPosition() { return { _posX, _posY, _posZ }; }

}