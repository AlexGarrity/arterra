#include "world/Chunk.hpp"

#include "world/World.hpp"

namespace arterra {

	Chunk::Chunk(int posX, int posY, int posZ, World* world)
		: _position { posX, posY, posZ }
		, _world { world }
	{
		_subChunks.reserve(SIZE_X * SIZE_Y * 0.25 * SIZE_Z);
		for (auto y = 0; y < SIZE_Y * 0.25; ++y) {
			for (auto z = 0; z < SIZE_Z; ++z) {
				for (auto x = 0; x < SIZE_X; ++x) {
					auto pos = BlockPosition(x, y, z);
					_subChunks.emplace(pos, SubChunk(x, y, z, this));
				}
			}
		}
	}

	Chunk::Chunk(const Chunk& other)
	{
		_position = other._position;
		_world = other._world;
		_subChunks = std::move(other._subChunks);
		for (auto& sc : _subChunks) {
			sc.second.SetParent(this);
		}
	}

	void Chunk::CreateSubChunk(int x, int y, int z)
	{
		auto pos = BlockPosition(x, y, z);
		if (_subChunks.find(pos) != _subChunks.end())
			return;
		_subChunks.emplace(pos, SubChunk(pos._x, pos._y, pos._z, this));
	}

	SubChunkMap& Chunk::GetSubChunks() { return _subChunks; }

	SubChunk* Chunk::GetSubChunk(int x, int y, int z)
	{
		auto scX = x / SubChunk::SIZE_X;
		auto scY = y / SubChunk::SIZE_Y;
		auto scZ = z / SubChunk::SIZE_Z;
		auto pos = BlockPosition(x, y, z);
		auto sc = _subChunks.find(pos);
		if (sc == _subChunks.end())
			_subChunks.emplace(pos, SubChunk(pos._x, pos._y, pos._z, this));
		sc = _subChunks.find(pos);
		return &sc->second;
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
			if (sc.second.Update(deltaTime)) {
				out.push_back(&sc.second);
			}
		}
		return { out.begin(), out.end() };
	}

}