#pragma once

#include "PCH.hpp"

#include "world/WorldPosition.hpp"
#include "world/SubChunk.hpp"

namespace arterra {

	class World;
	enum Direction;

	using SubChunkMap = std::unordered_map<int, SubChunk*>;

	class Chunk {
	public:
		Chunk() = default;
		Chunk(World *world);
		Chunk(int posX, int posZ, World *world);
		Chunk(const Chunk &other);
		~Chunk();

		void SetPosition(int posX, int posZ);
		void SetWorld(World *world);

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

		World* GetWorld();

	private:
		SubChunkMap _subChunks;

		World *_world;

		ChunkPosition _position;
	};

}