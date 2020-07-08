#include "world/Chunk.hpp"

namespace arterra {

	size_t GetChunkIndex(int x, int y, int z) {
		if (x < 0 || y < 0 || z < 0) return -1;
		if (x > 15 || y > 15 || z > 15) return -1;
		return x + (16 * (z + (16 * y)));
	}

	Chunk::Chunk(int posX, int posY, int posZ, CullableModel &model) 
	: _position{posX, posY, posZ} {
		_subChunks.reserve(16);
		for (auto i = 0; i < 16; ++i) {
			_subChunks.emplace_back(_position._x, _position._y + (16*i), _position._z, model, this);
		}
	}

	std::vector<SubChunk> &Chunk::GetSubChunks() {
		return _subChunks;
	}

	SubChunk *Chunk::GetSubChunk(int x, int y, int z) {
		auto scX = x / 16;
		auto scY = y / 16;
		auto scZ = z / 16;
		auto pos = GetChunkIndex(scX, scY, scZ);
		if (pos == -1) return nullptr;
		return &_subChunks[pos];
	}

	BlockPosition Chunk::GetPosition() {
		return _position;
	}

}