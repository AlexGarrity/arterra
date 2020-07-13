#pragma once

#include "PCH.hpp"

#include "world/Block.hpp"
#include "world/SubChunk.hpp"

namespace arterra {

	using SubChunkMap = std::unordered_map<BlockPosition, SubChunk, BlockPositionHash>;

	class World;

	class Chunk {
	public:
		static const size_t SIZE_X = 1;
		static const size_t SIZE_Y = 64;
		static const size_t SIZE_Z = 1;
		static const size_t SIZE = SIZE_X * SIZE_Y * SIZE_Z;

		Chunk() = default;
		Chunk(int posX, int posY, int posZ, World *world);
		Chunk(const Chunk &other);

		void CreateSubChunk(int x, int y, int z);

		SubChunk* GetSubChunk(int x, int y, int z);
		SubChunkMap& GetSubChunks();

		BlockPosition GetPosition();
		std::vector<SubChunk*> Update(float deltaTime);

		inline World *GetWorld() { return _world; }

	private:
		SubChunkMap _subChunks;

		World *_world;

		BlockPosition _position;
	};

}