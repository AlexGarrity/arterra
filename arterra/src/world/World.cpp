#include "world/World.hpp"

#include "block/BlockManager.hpp"
#include "world/Block.hpp"
#include "world/SubChunk.hpp"
#include "world/Chunk.hpp"
#include "world/TerrainGenerator.hpp"

#if defined(_MSC_VER)
	#if defined(_DEBUG)
		constexpr size_t WORLD_SIZE = 6;
	#else
		constexpr size_t WORLD_SIZE = 16;
	#endif
#else
	#if defined(NDEBUG)
		constexpr size_t WORLD_SIZE = 16;
	#else
		constexpr size_t WORLD_SIZE = 6;
	#endif
#endif

namespace arterra {

	int WorldToChunkSpace(int v, int axis)
	{
		if (v < 0)
			return ((v + 1) / axis) - 1;
		return v / axis;
	}

	World::World(TerrainGenerator* generator, BlockManager* blockManager)
	{
		_modifiedSubChunks.reserve(WORLD_SIZE * 8);
		ResizeLoadDistance(WORLD_SIZE);
		_emptyChunks.reserve(WORLD_SIZE);
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
		int cY = y / SubChunk::SIZE_Y - ((y < 0) ? 1 : 0);
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

		auto c = _chunks.emplace(ChunkPosition{ x, z }, new Chunk(x, z, this));
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
		ChunkPosition pos{ x, z };
		auto it = FindChunkCS(x, z);
		if (it == _chunks.end()) {
			Logger::Error("Tried to delete a chunk at ", x, ", ", z, " but the chunk could not be found");
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
		auto bPos = ChunkPosition{ cX, cZ };
		auto pos = _chunks.find(bPos);
		return pos;
	}

	ChunkMap::iterator World::FindChunkCS(int x, int z)
	{
		auto bPos = ChunkPosition{ x, z };
		auto pos = _chunks.find(bPos);
		return pos;
	}

	void World::Update(float deltaTime, glm::vec3 playerPos)
	{
		DeleteOldChunks(playerPos);
		GenerateNewChunks(playerPos);

		auto& newChunks = _terrainGenerator->GetCompletedChunks();
		for (auto c : newChunks) { for (auto sc : c->GetSubChunks()) { sc.second->SetUpdated(true); } }
		newChunks.clear();

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
			_terrainGenerator->AddChunkToGeneratorQueue(c);
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
			auto chunkPos = c.second->GetPositionRaw();
			if (!ChunkInLoadDistance(chunkPos, playerPos)) {
				for (auto subChunk : c.second->GetSubChunks()) {
					_emptyChunks.emplace_back(subChunk.second->GetPosition());
				}
				deletedChunks.emplace_back(chunkPos);
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

	float World::DistanceToChunk(glm::vec3 pos, ChunkPosition c) { return sqrtf(DistanceToChunk2(pos, c)); }

	float World::DistanceToChunk2(glm::vec3 pos, ChunkPosition c)
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

	float World::DistanceToChunk(ChunkPosition a, ChunkPosition c) { return sqrtf(DistanceToChunk2(a, c)); }

	float World::DistanceToChunk2(ChunkPosition a, ChunkPosition c)
	{
		auto deltaX = c._x - a._x;
		deltaX *= deltaX;
		auto deltaY = c._z - a._z;
		deltaY *= deltaY;

		return static_cast<float>(deltaX + deltaY);
	}

	void World::ResizeLoadDistance(float distance)
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
