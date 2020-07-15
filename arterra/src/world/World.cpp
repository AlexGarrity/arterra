#include "world/World.hpp"

namespace arterra {

	World::World() { _modifiedSubChunks.reserve(128); }

	int WorldToChunkSpace(int v, int axis)
	{
		if (v < 0)
			return ((v + 1) / axis) - 1;
		return v / axis;
	}

	Chunk* World::CreateChunk(int x, int y, int z)
	{
		auto cX = WorldToChunkSpace(x, SubChunk::SIZE_X);
		auto cZ = WorldToChunkSpace(z, SubChunk::SIZE_Z);

		return CreateChunkCS(cX, cZ);
	}

	Chunk* World::GetChunk(int x, int y, int z)
	{
		auto cX = WorldToChunkSpace(x, SubChunk::SIZE_X);
		auto cZ = WorldToChunkSpace(z, SubChunk::SIZE_Z);

		return GetChunkCS(cX, cZ);
	}

	SubChunk* World::GetSubChunk(int x, int y, int z)
	{
		int cX = WorldToChunkSpace(x, SubChunk::SIZE_X);
		int cY;
		if (y < 0)
			cY = (y - SubChunk::SIZE_Y) / SubChunk::SIZE_Y;
		else
			cY = y / SubChunk::SIZE_Y;
		int cZ = WorldToChunkSpace(z, SubChunk::SIZE_Z);

		return GetSubChunkCS(cX, cY, cZ);
	}

	Chunk* World::CreateChunkCS(int x, int z)
	{
		auto chunk = FindChunkCS(x, z);
		if (chunk != _chunks.end())
			return &chunk->second;
		_chunks.emplace(WorldPosition { x, 0, z }, Chunk { x, z, this });
		return GetChunkCS(x, z);
	}

	Chunk* World::GetChunkCS(int x, int z)
	{
		auto chunk = FindChunkCS(x, z);
		if (chunk == _chunks.end())
			return nullptr;
		return &chunk->second;
	}

	SubChunk* World::GetSubChunkCS(int x, int y, int z)
	{
		auto chunk = GetChunkCS(x, z);
		if (!chunk)
			return nullptr;
		return chunk->GetSubChunkCS(y);
	}

	Block* World::GetBlock(int x, int y, int z)
	{
		auto sc = GetSubChunk(x, y, z);
		if (!sc)
			return nullptr;
		return sc->GetBlock(x, y, z);
	}

	ChunkMap::iterator World::FindChunk(int x, int z)
	{
		auto cX = WorldToChunkSpace(x, SubChunk::SIZE_X);
		auto cZ = WorldToChunkSpace(z, SubChunk::SIZE_Z);
		auto bPos = WorldPosition { cX, 0, cZ };
		auto pos = _chunks.find(bPos);
		return pos;
	}

	ChunkMap::iterator World::FindChunkCS(int x, int z)
	{
		auto bPos = WorldPosition { x, 0, z };
		auto pos = _chunks.find(bPos);
		return pos;
	}

	void World::Update(float deltaTime)
	{
		_modifiedSubChunks.clear();
		for (auto& chunk : _chunks) {
			auto updatedSubChunks = chunk.second.Update(deltaTime);
			_modifiedSubChunks.insert(_modifiedSubChunks.end(), updatedSubChunks.begin(), updatedSubChunks.end());
		}
	}

	std::vector<SubChunk*>& World::GetModifiedSubChunks() { return _modifiedSubChunks; }

}