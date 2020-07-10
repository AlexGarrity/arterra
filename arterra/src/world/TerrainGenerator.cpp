#include "world/TerrainGenerator.hpp"

namespace arterra {

	TerrainGenerator::TerrainGenerator() {

	}

	void TerrainGenerator::GenerateChunk(Chunk &out, Block &block) {
		for (auto &subChunk : out.GetSubChunks()) {
			for (auto y = 0; y < 16; ++y) {
				for (auto z = 0; z < 16; ++z) {
					for (auto x = 0; x < 16; ++x) {
						auto cp = subChunk.GetPosition();

						auto bx = cp._x + x;
						auto by = cp._y + y;
						auto bz = cp._z + z;

						auto d = (1.0f / 5.0f);
						auto fx = static_cast<float>(bx) * d;
						auto fy = static_cast<float>(by) * d;
						auto fz = static_cast<float>(bz) * d;

						auto v = _generator.Generate(fx, fy, fz);
						if (v > 0.1f) {
							subChunk.SetBlock(x, y, z, block);
						}
					}
				}
			}
		}

	}



}