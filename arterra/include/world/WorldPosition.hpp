#pragma once

namespace arterra {

	struct WorldPosition {
		int _x = 0;
		int _y = 0;
		int _z = 0;

		WorldPosition() = default;
		WorldPosition(int x, int y, int z);
		WorldPosition(const WorldPosition& other);

		void operator=(const WorldPosition& other);
		bool operator==(const WorldPosition& other) const;
		
	};

	struct WorldPositionHash {
		size_t operator()(const WorldPosition& pos) const
		{
			return (pos._x * 131071) ^ (pos._y * 524287) ^ (pos._z * 8191);
		}
	};

}