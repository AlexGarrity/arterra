#include "world/TerrainGenerator.hpp"

namespace arterra {

	TerrainGenerator::TerrainGenerator() {}

	void TerrainGenerator::GenerateChunk(Chunk& out, Block& block)
	{
		std::vector<uint16_t> _heightMap;
		_heightMap.resize(256);
		auto cp = out.GetPosition();
		for (auto z = 0; z < 16; ++z) {
			for (auto x = 0; x < 16; ++x) {
				auto fX = static_cast<float>(x + cp._x) * 0.02f;
				auto fZ = static_cast<float>(z + cp._z) * 0.02f;
				float gx = _generator2D.Generate(fX, fZ);
				float go = (gx + 3.2f) * 0.2f;
				uint16_t h = static_cast<uint16_t>(go * 72.0f);
				_heightMap[x + 16 * z] = h;
			}
		}

		for (auto& sc : out.GetSubChunks()) {
			for (auto z = 0; z < 16; ++z) {
				for (auto x = 0; x < 16; ++x) {
					auto height = _heightMap[x + 16 * z];
					auto pos = sc.GetPosition();
					auto dY = std::max<int>(std::min<int>(height - pos._y, 16), 0);
					for (auto y = 0; y < dY; ++y) {
						sc.SetBlock(x, y, z, block);
					}
				}
			}
		}
	}

}