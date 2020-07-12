#include "world/Chunk.hpp"

#include "world/World.hpp"

namespace arterra {

	size_t GetChunkIndex(int x, int y, int z)
	{
		if (x < 0 || y < 0 || z < 0)
			return -1;
		if (x > Chunk::SIZE_X - 1 || y > Chunk::SIZE_Y - 1 || z > Chunk::SIZE_Z - 1)
			return -1;
		return x + (Chunk::SIZE_X * (z + (Chunk::SIZE_Z * y)));
	}

	Chunk::Chunk(int posX, int posY, int posZ, World* world)
		: _position { posX, posY, posZ }
		, _world { world }
	{
		_subChunks.reserve(SIZE);
		for (auto y = 0; y < SIZE_Y; ++y) {
			for (auto z = 0; z < SIZE_Z; ++z) {
				for (auto x = 0; x < SIZE_X; ++x) {
					_subChunks.emplace_back(x, y, z, this);
				}
			}
		}
	}

	Chunk::Chunk(const Chunk& other)
	{
		_position = other._position;
		_world = other._world;
		_subChunks.insert(_subChunks.end(), other._subChunks.begin(), other._subChunks.end());
		for (auto& sc : _subChunks) {
			sc.SetParent(this);
		}
	}

	std::vector<SubChunk>& Chunk::GetSubChunks() { return _subChunks; }

	SubChunk* Chunk::GetSubChunk(int x, int y, int z)
	{
		auto scX = x / SubChunk::SIZE_X;
		auto scY = y / SubChunk::SIZE_Y;
		auto scZ = z / SubChunk::SIZE_Z;
		auto pos = GetChunkIndex(scX, scY, scZ);
		if (pos == -1)
			return nullptr;
		return &_subChunks[pos];
	}

	BlockPosition Chunk::GetPosition()
	{
		return { _position._x * static_cast<int>(SubChunk::SIZE_X),
			_position._y * static_cast<int>(SubChunk::SIZE_Y), _position._z * static_cast<int>(SubChunk::SIZE_Z) };
	}

	std::vector<SubChunk*> Chunk::Update(float deltaTime)
	{
		std::vector<SubChunk*> out;
		out.reserve(4);
		for (auto& sc : _subChunks) {
			if (sc.Update(deltaTime)) {
				out.push_back(&sc);
			}
		}
		return { out.begin(), out.end() };
	}

}