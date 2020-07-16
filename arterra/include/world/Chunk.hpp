#pragma once

#include "PCH.hpp"

#include "world/Block.hpp"
#include "world/SubChunk.hpp"

namespace arterra {

	using SubChunkMap = std::unordered_map<int, SubChunk>;

	class World;

	class Chunk {
	public:
		Chunk() = default;
		Chunk(int posX, int posZ, World *world);
		Chunk(const Chunk &other);

		void CreateSubChunk(int y);
		void CreateSubChunksToHeight(int height);
		
		void CreateSubChunkCS(int y);
		void CreateSubChunksToHeightCS(int height);

		SubChunk* GetSubChunk(int y);
		SubChunk* GetSubChunkCS(int y);

		void UpdateNeighbours();
		void UpdateBlocks();

		void UpdateBorder(Direction borderDirection);
		void UpdateSubChunkBorder(int y, Direction borderDirection);
		void UpdateSubChunkBorderSC(int y, Direction borderDirection);

		SubChunkMap& GetSubChunks();

		WorldPosition GetPosition();
		ChunkPosition GetPositionRaw();
		std::vector<SubChunk*> Update(float deltaTime);

		inline World *GetWorld() { return _world; }

	private:
		SubChunkMap _subChunks;

		World *_world;

		ChunkPosition _position;
	};

}