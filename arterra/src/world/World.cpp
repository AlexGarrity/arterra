#include "world/World.hpp"

namespace arterra {

	World::World() {
		_modifiedSubChunks.reserve(128);
	}

	Chunk* World::CreateChunk(int x, int y, int z, CullableModel& model)
	{
		return CreateChunk(x / 16, z / 16, model);
	}

	Chunk* World::CreateChunk(int x, int z, CullableModel& model)
	{
		auto chunk = FindChunk(x, z);
		if (chunk != _chunks.end())
			return &chunk->second;
		auto pos = BlockPosition(x, 0, z);
		_chunks.emplace(std::make_pair(pos, Chunk{pos._x, pos._y, pos._z, this, model}));
		return GetChunk(x, z);
	}

	Chunk* World::GetChunk(int x, int y, int z) { return GetChunk(x / 16, z / 16); }

	Chunk* World::GetChunk(int x, int z)
	{
		auto chunk = FindChunk(x, z);
		if (chunk == _chunks.end())
			return nullptr;
		return &chunk->second;
	}

	SubChunk* World::GetSubChunk(int x, int y, int z)
	{
		auto chunk = GetChunk(x, z);
		if (!chunk)
			return nullptr;
		return chunk->GetSubChunk(x, y, z);
	}

	Block* World::GetBlock(int x, int y, int z)
	{
		auto sc = GetSubChunk(x, y, z);
		if (!sc)
			return nullptr;
		return sc->GetBlock(x % 16, y % 16, z % 16);
	}

	ChunkMap::iterator World::FindChunk(int x, int z)
	{
		auto bPos = BlockPosition { x, 0, z };
		auto pos = _chunks.find(bPos);
		return pos;
	}

	void World::Update(float deltaTime) {
		_modifiedSubChunks.clear();
		for (auto &chunk : _chunks) {
			auto updatedSubChunks = chunk.second.Update(deltaTime);
			_modifiedSubChunks.insert(_modifiedSubChunks.end(), updatedSubChunks.begin(), updatedSubChunks.end());
		}
	}

	std::vector<SubChunk*> &World::GetModifiedSubChunks() {
		return _modifiedSubChunks;
	}

}