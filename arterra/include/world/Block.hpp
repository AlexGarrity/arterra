#pragma once

#include "PCH.hpp"
#include "block/BlockData.hpp"
#include "world/WorldPosition.hpp"

namespace arterra {

	class World;
	class SubChunk;

	class Block {

	public:
		Block(const Block& other);
		Block(int posX, int posY, int posZ, SubChunk* subChunk, BlockData& blockData);

		void operator=(const Block& other);

		WorldPosition GetPosition() const;
		WorldPosition GetPositionRaw() const;
		BlockData& GetData() const;

		inline bool IsVisible() const { return _visible; }

		std::array<bool, 6> GetVisibleFaces() const;

		void Update(size_t width);
		void UpdateVisiblity(std::array<Block*, 6>& neighbours);
		std::array<Block*, 6> GetNeighbours();

		void SetParent(SubChunk* subChunk);

	private:
		bool _visible;
		std::array<bool, 6> _visibleFaces;

		BlockData& _blockData;
		SubChunk* _subChunk;
		World* _world;
		WorldPosition _position;
	};

}