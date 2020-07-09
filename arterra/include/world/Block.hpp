#pragma once

#include "PCH.hpp"
#include "model/CullableModel.hpp"

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

		bool operator==(const BlockPosition& other) const
		{
			return _x == other._x && _y == other._y && _z == other._z;
		}
	};

	class SubChunk;

	class Block {

	public:
		Block(const Block& other);
		Block(int posX, int posY, int posZ, CullableModel& model, SubChunk* subChunk);

		BlockPosition GetPosition() const;
		BlockPosition GetPositionRaw() const;
		CullableModel& GetModel() const;

		inline bool IsVisible() const { return _visible; }

		std::array<bool, 6> GetVisibleFaces() const;

		void Update(size_t width);
		void UpdateVisiblity();
		void UpdateNeighbours();

		void SetParent(SubChunk *subChunk);

	private:
		bool _visible;
		std::array<bool, 6> _visibleFaces;
		std::array<Block*, 6> _neighbours;

		CullableModel& _model;
		SubChunk* _subChunk;
		const BlockPosition _position;
	};

}