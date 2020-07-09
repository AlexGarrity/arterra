#pragma once

#include "PCH.hpp"
#include "world/Block.hpp"

namespace arterra {

	class Chunk;

	class SubChunk {

	public:
		SubChunk(int posX, int posY, int posZ, CullableModel& model, Chunk* parent);
		SubChunk(const SubChunk& other);
		~SubChunk() {}
		void SetParent(Chunk* chunk);

		std::array<Block*, 4096>& GetBlocks();
		Block* GetBlock(int x, int y, int z) const;

		void SetBlock(int x, int y, int z, Block& block);
		void DeleteBlock(int x, int y, int z);

		BlockPosition GetPosition();
		inline Chunk* GetChunk() { return _chunk; };

		bool Update(float deltaTime);

	private:
		BlockPosition _position;
		Chunk* _chunk;
		bool _updated = true;

		// Contains 16 x 16 x 16 blocks.
		std::array<Block*, 4096> _blocks;
	};

}