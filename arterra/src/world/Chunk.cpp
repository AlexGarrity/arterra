#include "world/Chunk.hpp"

#include "world/World.hpp"

namespace arterra {

	Chunk::Chunk(int posX, int posZ, World* world)
		: _posX { posX }
		, _posZ { posZ }
		, _world { world }
	{
	}

	Chunk::Chunk(const Chunk& other)
	{
		_posX = other._posX;
		_posZ = other._posZ;
		_world = other._world;
		_subChunks = std::move(other._subChunks);
		for (auto& sc : _subChunks) {
			sc.second.SetParent(this);
		}
	}

	void Chunk::CreateSubChunk(int y) { CreateSubChunkCS(y / SubChunk::SIZE_Y); }

	void Chunk::CreateSubChunksToHeight(int height) { CreateSubChunksToHeightCS(height / SubChunk::SIZE_Y); }

	void Chunk::CreateSubChunkCS(int y)
	{
		if (_subChunks.find(y) != _subChunks.end())
			return;
		_subChunks.emplace(y, SubChunk(y, this));
	}

	void Chunk::CreateSubChunksToHeightCS(int height)
	{
		for (auto iY = 0; iY <= height; ++iY) {
			_subChunks.emplace(iY, SubChunk { iY, this });
		}
	}

	SubChunkMap& Chunk::GetSubChunks() { return _subChunks; }

	SubChunk* Chunk::GetSubChunk(int y) { return GetSubChunkCS(y / SubChunk::SIZE_Y); }

	SubChunk* Chunk::GetSubChunkCS(int y)
	{
		auto it = _subChunks.find(y);
		if (it == _subChunks.end())
			return nullptr;
		return &it->second;
	}

	WorldPosition Chunk::GetPosition() { return { _posX * SubChunk::SIZE_X, 0, _posZ * SubChunk::SIZE_Z }; }

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