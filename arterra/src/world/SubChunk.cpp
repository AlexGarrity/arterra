#include "world/SubChunk.hpp"

#include "world/Chunk.hpp"
#include "world/World.hpp"

#include "util/Maths.hpp"

namespace arterra {

	using Maths::PMod;

	size_t ResolveArrayPosition(int x, int y, int z)
	{
		if (x > SubChunk::SIZE_X - 1 || y > SubChunk::SIZE_Y - 1 || z > SubChunk::SIZE_Z - 1)
			return -1;
		if (x < 0 || y < 0 || z < 0)
			return -1;
		return x + (SubChunk::SIZE_X * (z + (SubChunk::SIZE_Z * y)));
	}

	SubChunk::SubChunk(int posY, Chunk* parent)
		: _position { posY }
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

	SubChunk::~SubChunk()
	{
		for (auto b : _blocks) {
			if (b)
				delete b;
		}
	}

	void SubChunk::SetParent(Chunk* chunk) { _chunk = chunk; }

	std::array<Block*, SubChunk::SIZE>& SubChunk::GetBlocks() { return _blocks; }

	Block* SubChunk::GetBlock(int x, int y, int z) const
	{
		auto px = PMod(x, SIZE_X);
		auto py = PMod(y, SIZE_Y);
		auto pz = PMod(z, SIZE_Z);

		return GetBlockCS(px, py, pz);
	}

	void SubChunk::SetBlock(int x, int y, int z, BlockData& data)
	{
		auto px = PMod(x, SIZE_X);
		auto py = PMod(y, SIZE_Y);
		auto pz = PMod(z, SIZE_Z);

		return SetBlockCS(px, py, pz, data);
	}

	void SubChunk::DeleteBlock(int x, int y, int z)
	{
		auto px = PMod(x, SIZE_X);
		auto py = PMod(y, SIZE_Y);
		auto pz = PMod(z, SIZE_Z);

		return DeleteBlockCS(px, py, pz);
	}

	Block* SubChunk::GetBlockCS(int x, int y, int z) const
	{
		auto pos = ResolveArrayPosition(x, y, z);
		return (pos == -1) ? nullptr : _blocks[pos];
	}

	void SubChunk::SetBlockCS(int x, int y, int z, BlockData& data)
	{
		auto pos = ResolveArrayPosition(x, y, z);
		if (pos == -1)
			return;
		if (_blocks[pos])
			delete _blocks[pos];
		_blocks[pos] = new Block(x, y, z, this, data);
		_updated = true;
	}

	void SubChunk::DeleteBlockCS(int x, int y, int z)
	{
		auto pos = ResolveArrayPosition(x, y, z);
		if (pos == -1)
			return;
		delete _blocks[pos];
	}

	void SubChunk::UpdateBorder(Direction borderDirection)
	{
		enum eB { LowerX, UpperX, LowerY, UpperY, LowerZ, UpperZ };
		std::array<int, 6> bounds = { 0, 0, 0, 0, 0, 0 };
		switch (borderDirection) {
			case Direction::PosX:
				bounds = { SIZE_X - 1, SIZE_X, 0, SIZE_Y, 0, SIZE_Z };
				break;
			case Direction::NegX:
				bounds = { 0, 1, 0, SIZE_Y, 0, SIZE_Z };
				break;
			case Direction::PosY:
				bounds = { 0, SIZE_X, SIZE_Y - 1, SIZE_Y, 0, SIZE_Z };
				break;
			case Direction::NegY:
				bounds = { 0, SIZE_X, 0, 1, 0, SIZE_Z };
				break;
			case Direction::PosZ:
				bounds = { 0, SIZE_X, 0, SIZE_Y, SIZE_Z - 1, SIZE_Z };
				break;
			case Direction::NegZ:
				bounds = { 0, SIZE_X, 0, SIZE_Y, 0, 1 };
				break;
			case Direction::Always:
				// Do nothing, I guess
				// Should probably throw an error
				break;
		}

		for (auto y = bounds[LowerY]; y < bounds[UpperY]; ++y) {
			for (auto z = bounds[LowerZ]; z < bounds[UpperZ]; ++z) {
				for (auto x = bounds[LowerX]; x < bounds[UpperX]; ++x) {
					auto b = GetBlockCS(x, y, z);
					if (b)
						b->Update(0);
				}
			}
		}
		_updated = true;
	}

	WorldPosition SubChunk::GetPosition()
	{
		auto cp = _chunk->GetPosition();
		return { cp._x, (_position * static_cast<int>(SIZE_Y)), cp._z };
	}

	int SubChunk::GetPositionRaw() { return _position; }
	
	Chunk* SubChunk::GetChunk() { return _chunk; }

	bool SubChunk::Update(float deltaTime)
	{
		if (_updated) {
			_updated = false;
			return true;
		}
		return false;
	}

	bool SubChunk::IsUpdated() const { return _updated; }
	
	void SubChunk::SetUpdated(bool b) { _updated = b; }
}
