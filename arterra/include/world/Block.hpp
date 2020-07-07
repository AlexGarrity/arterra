#pragma once

#include "PCH.hpp"
#include "model/CullableModel.hpp"

namespace arterra {

	struct BlockPosition {
		int _x;
		int _y;
		int _z;

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
	};

	class SubChunk;

	class Block {

	public:
		Block(const Block& other);
		Block(int posX, int posY, int posZ, CullableModel& model, SubChunk* subChunk);

		BlockPosition GetPosition() const;
		BlockPosition GetPositionRaw() const;
		CullableModel& GetModel() const;

		std::array<bool, 6> GetVisibleFaces() const;

	private:
		std::array<bool, 6> _visibleFaces;

		CullableModel& _model;
		const SubChunk* _subChunk;
		const BlockPosition _position;
	};

}