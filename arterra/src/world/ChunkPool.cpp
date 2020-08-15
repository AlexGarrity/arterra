#include "world/ChunkPool.hpp"

#include "world/Chunk.hpp"

namespace arterra {

	ChunkPool::Handle::Handle() { _chunk = new Chunk(); }

	ChunkPool::Handle::~Handle() { delete _chunk; }

	ChunkPool::ChunkPool() {}

	void ChunkPool::SetSize(size_t size)
	{
		_pool.resize(size);
	}

	ChunkPool::Handle* ChunkPool::GetEmptyChunk()
	{
		for (auto& c : _pool) {
			if (c._state == ChunkPool::Handle::State::Empty) {
				return &c;
			}
		}
		return nullptr;
	}

	ChunkPool::Handle* ChunkPool::FindHandle(Chunk* chunk)
	{
		if (!chunk)
			return nullptr;
		for (auto& c : _pool) {
			if (c._chunk == chunk)
				return &c;
		}
		return nullptr;
	}

	std::vector<ChunkPool::Handle>& ChunkPool::GetChunks() { return _pool; }

}