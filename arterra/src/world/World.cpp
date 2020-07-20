#include "world/World.hpp"

#include "block/BlockManager.hpp"
#include "world/Block.hpp"
#include "world/Chunk.hpp"
#include "world/SubChunk.hpp"
#include "world/TerrainGenerator.hpp"

namespace arterra {

	int WorldToChunkSpace(int v, int axis)
	{
		if (v < 0)
			return ((v + 1) / axis) - 1;
		return v / axis;
	}

	World::World(TerrainGenerator* generator, BlockManager* blockManager)
	{
		_modifiedSubChunks.reserve(128);
		ResizeLoadDistance(16);
		_emptyChunks.reserve(16);
		_terrainGenerator = generator;
		_blockManager = blockManager;
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

	void World::DeleteChunk(int x, int z)
	{
		auto cX = WorldToChunkSpace(x, SubChunk::SIZE_X);
		auto cZ = WorldToChunkSpace(z, SubChunk::SIZE_Z);
		DeleteChunkCS(cX, cZ);
	}

	Chunk* World::CreateChunkCS(int x, int z)
	{
		// Check that the chunk doesn't already exist
		auto chunk = FindChunkCS(x, z);
		if (chunk != _chunks.end())
			return nullptr;

		auto c = _chunks.emplace(ChunkPosition { x, z }, new Chunk(x, z, this));
		return c.first->second;
	}

	Chunk* World::GetChunkCS(int x, int z)
	{
		auto chunk = FindChunkCS(x, z);
		if (chunk == _chunks.end())
			return nullptr;
		return chunk->second;
	}

	SubChunk* World::GetSubChunkCS(int x, int y, int z)
	{
		auto chunk = GetChunkCS(x, z);
		if (!chunk)
			return nullptr;
		return chunk->GetSubChunkCS(y);
	}

	void World::DeleteChunkCS(int x, int z)
	{
		ChunkPosition pos { x, z };
		auto it = FindChunkCS(x, z);
		if (it == _chunks.end()) {
			Logger::Get().Log(
				Logger::Error, "Tried to delete a chunk at ", x, ", ", z, " but the chunk could not be found");
			return;
		}
		delete it->second;
		_chunks.erase(it);
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
		auto bPos = ChunkPosition { cX, cZ };
		auto pos = _chunks.find(bPos);
		return pos;
	}

	ChunkMap::iterator World::FindChunkCS(int x, int z)
	{
		auto bPos = ChunkPosition { x, z };
		auto pos = _chunks.find(bPos);
		return pos;
	}

	void World::Update(float deltaTime, glm::vec3 playerPos)
	{
		DeleteOldChunks(playerPos);
		GenerateNewChunks(playerPos);

		_modifiedSubChunks.clear();
		for (auto& chunk : _chunks) {
			if (!chunk.second)
				continue;

			auto updatedSubChunks = chunk.second->Update(deltaTime);
			_modifiedSubChunks.insert(_modifiedSubChunks.end(), updatedSubChunks.begin(), updatedSubChunks.end());
		}
	}

	void World::GenerateNewChunks(glm::vec3 playerPosition)
	{
		std::vector<Chunk*> newChunks;
		newChunks.reserve(static_cast<int>(_loadDistance));

		// Figure out if any new chunks need generating
		auto playerPos = ChunkPosition(WorldToChunkSpace(playerPosition.x, SubChunk::SIZE_X),
			WorldToChunkSpace(playerPosition.z, SubChunk::SIZE_Z));

		auto startX = playerPos._x - static_cast<int>(_loadDistance);
		auto startZ = playerPos._z - static_cast<int>(_loadDistance);

		auto endX = playerPos._x + static_cast<int>(_loadDistance);
		auto endZ = playerPos._z + static_cast<int>(_loadDistance);

		for (auto z = startZ; z < endZ; ++z) {
			for (auto x = startX; x < endX; ++x) {
				if (ChunkInLoadDistance(ChunkPosition(x, z), playerPos))
					newChunks.push_back(CreateChunkCS(x, z));
			}
		}

		for (auto& c : newChunks) {
			if (!c)
				continue;
			_terrainGenerator->GenerateChunk(*c, *_blockManager);
		}
	}

	void World::DeleteOldChunks(glm::vec3 playerPosition)
	{
		std::vector<ChunkPosition> deletedChunks;
		deletedChunks.reserve(static_cast<int>(_loadDistance));
		_emptyChunks.clear();
		// Figure out if any new chunks need generating
		auto playerPos = ChunkPosition(WorldToChunkSpace(playerPosition.x, SubChunk::SIZE_X),
			WorldToChunkSpace(playerPosition.z, SubChunk::SIZE_Z));

		for (auto& c : _chunks) {
			if (!ChunkInLoadDistance(c.second->GetPositionRaw(), playerPos)) {
				_emptyChunks.push_back(c.second->GetPosition());
				deletedChunks.push_back(ChunkPosition(c.second->GetPositionRaw()));
			}
		}
		for (auto& d : deletedChunks) {
			delete _chunks[d];
			_chunks.erase(d);
		}
	}

	bool World::ChunkInLoadDistance(glm::vec3 pos, ChunkPosition c)
	{
		return (DistanceToChunk2(pos, c) <= _loadDistance2);
	}

	float_t World::DistanceToChunk(glm::vec3 pos, ChunkPosition c) { return sqrtf(DistanceToChunk2(pos, c)); }

	float_t World::DistanceToChunk2(glm::vec3 pos, ChunkPosition c)
	{
		auto deltaX = pos.x - (c._x * 16.0f);
		deltaX *= deltaX;
		auto deltaY = pos.y - (c._z * 16.0f);
		deltaY *= deltaY;

		return deltaX + deltaY;
	}

	bool World::ChunkInLoadDistance(ChunkPosition a, ChunkPosition c)
	{
		return (DistanceToChunk2(a, c) <= _loadDistance2);
	}

	float_t World::DistanceToChunk(ChunkPosition a, ChunkPosition c) { return sqrtf(DistanceToChunk2(a, c)); }

	float_t World::DistanceToChunk2(ChunkPosition a, ChunkPosition c)
	{
		auto deltaX = c._x - a._x;
		deltaX *= deltaX;
		auto deltaY = c._z - a._z;
		deltaY *= deltaY;

		return static_cast<float_t>(deltaX + deltaY);
	}

	void World::ResizeLoadDistance(float_t distance)
	{
		_loadDistance = distance;
		_loadDistance2 = distance * distance;

		// Recalculate the number of points
		size_t chunks = (_loadDistance + 2) * (_loadDistance + 2);
		_chunkPool.SetSize(chunks);
	}

	std::vector<SubChunk*>& World::GetModifiedSubChunks() { return _modifiedSubChunks; }

	std::vector<WorldPosition>& World::GetEmptyChunks() { return _emptyChunks; }

}