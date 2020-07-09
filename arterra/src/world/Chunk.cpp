#include "world/Chunk.hpp"

#include "world/World.hpp"

namespace arterra {

	size_t GetChunkIndex(int x, int y, int z)
	{
		if (x < 0 || y < 0 || z < 0)
			return -1;
		if (x > 16 || y > 16 || z > 16)
			return -1;
		return x + (1 * (z + (1 * y)));
	}

	Chunk::Chunk(int posX, int posY, int posZ, World *world, CullableModel& model)
		: _position { posX, posY, posZ }, _world{world}
	{
		_subChunks.reserve(16);
		for (auto y = 0; y < 4; ++y) {
			_subChunks.emplace_back(0, y, 0, model, this);
		}
	}

	Chunk::Chunk(const Chunk &other) {
		_position = other._position;
		_world = other._world;
		_subChunks.insert(_subChunks.end(), other._subChunks.begin(), other._subChunks.end());
		for (auto &sc : _subChunks) {
			sc.SetParent(this);
		}
	}

	std::vector<SubChunk>& Chunk::GetSubChunks() { return _subChunks; }

	SubChunk* Chunk::GetSubChunk(int x, int y, int z)
	{
		auto scX = x / 16;
		auto scY = y / 16;
		auto scZ = z / 16;
		auto pos = GetChunkIndex(scX, scY, scZ);
		if (pos == -1)
			return nullptr;
		return &_subChunks[pos];
	}

	BlockPosition Chunk::GetPosition() { return {_position._x * 16, _position._y * 16, _position._z * 16}; }

	std::vector<SubChunk*> Chunk::Update(float deltaTime) {
		std::vector<SubChunk*> out;
		out.reserve(4);
		for (auto &sc : _subChunks) {
			if (sc.Update(deltaTime)) {
				out.push_back(&sc);
			}
		}
		return {out.begin(), out.end()};
	}

}