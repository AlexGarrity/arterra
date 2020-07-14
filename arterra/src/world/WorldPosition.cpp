#include "world/WorldPosition.hpp"

namespace arterra {

	WorldPosition::WorldPosition(int x, int y, int z)
		: _x(x)
		, _y(y)
		, _z(z)
	{
	}

	WorldPosition::WorldPosition(const WorldPosition& other)
	{
		_x = other._x;
		_y = other._y;
		_z = other._z;
	}

	void WorldPosition::operator=(const WorldPosition& other)
	{
		_x = other._x;
		_y = other._y;
		_z = other._z;
	}

	bool WorldPosition::operator==(const WorldPosition &other) const {
		return _x == other._x && _y == other._y && _z == other._z;
	}

}