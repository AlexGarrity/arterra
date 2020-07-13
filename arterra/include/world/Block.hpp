#pragma once

#include "PCH.hpp"
#include "block/BlockData.hpp"

namespace arterra {

	struct BlockPosition {
		int _x = 0;
		int _y = 0;
		int _z = 0;

		BlockPosition() = default;

		BlockPosition(int x, int y, int z)
			: _x(x)
			, _y(y)
			, _z(z)
		{
		}

		BlockPosition(const BlockPosition& other)
		{
			_x = other._x;
			_y = other._y;
			_z = other._z;
		}

		void operator=(const BlockPosition& other)
		{
			_x = other._x;
			_y = other._y;
			_z = other._z;
		}

		bool operator==(const BlockPosition& other) const
		{
			return _x == other._x && _y == other._y && _z == other._z;
		}
	};

	struct BlockPositionHash {
		size_t operator()(const BlockPosition& pos) const
		{
			return std::hash<int> {}(pos._x) ^ std::hash<int> {}(pos._y) ^ std::hash<int> {}(pos._z);
		}
	};

	class SubChunk;

	class Block {

	public:
		Block(const Block& other);
		Block(int posX, int posY, int posZ, SubChunk* subChunk, BlockData& blockData);

		void operator=(const Block& other);

		BlockPosition GetPosition() const;
		BlockPosition GetPositionRaw() const;
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
		BlockPosition _position;
	};

}