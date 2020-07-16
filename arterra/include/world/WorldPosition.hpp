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

	struct ChunkPosition {
		int _x = 0;
		int _z = 0;

		ChunkPosition() = default;
		ChunkPosition(int x, int z);
		ChunkPosition(const ChunkPosition& other);

		void operator=(const ChunkPosition& other);
		bool operator==(const ChunkPosition& other) const;
	};

	struct WorldPositionHash {
		size_t operator()(const WorldPosition& pos) const
		{
			return (pos._x * 131071) + (pos._y * 524287) + (pos._z * 8191);
		}
	};

	struct ChunkPositionHash {
		size_t operator()(const ChunkPosition& pos) const
		{
			return (pos._x * 131071) ^ (pos._z * 8191);
		}
	};

}