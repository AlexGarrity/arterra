#pragma once

#include "PCH.hpp"

namespace arterra {

	class Chunk;

	class ChunkPool {
	public:
		struct Handle {
			enum class State { Empty, Generate, Active, Delete };
			Chunk* _chunk { nullptr };
			State _state { State::Empty };

			Handle();
			~Handle();
		};

	public:
		ChunkPool();

		void SetSize(size_t size);

		Handle* GetEmptyChunk();
		Handle* FindHandle(Chunk* chunk);

		std::vector<Handle>& GetChunks();

	private:
		std::vector<Handle> _pool;
	};

}