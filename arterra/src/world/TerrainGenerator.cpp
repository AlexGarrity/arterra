#include "world/TerrainGenerator.hpp"

namespace arterra {

	TerrainGenerator::TerrainGenerator() {}

	void TerrainGenerator::GenerateChunk(Chunk& out, BlockManager& blockManager)
	{
		std::vector<uint16_t> _heightMap;
		_heightMap.resize(256);
		auto cp = out.GetPosition();
		for (auto z = 0; z < SubChunk::SIZE_Z * Chunk::SIZE_Z; ++z) {
			for (auto x = 0; x < SubChunk::SIZE_X * Chunk::SIZE_X; ++x) {
				auto fX = static_cast<float>(x + cp._x) / 64.0f;
				auto fZ = static_cast<float>(z + cp._z) / 64.0f;
				float g1 = glm::simplex(glm::vec2(fX, fZ)) * 0.0625f;
				float g2 = glm::simplex(glm::vec2(fX * 0.25f, fZ * 0.25f)) * 0.25f;
				float g3 = glm::simplex(glm::vec2(fX * 0.125f, fZ * 0.125f)) * 0.5f;
				float go = g1 + g2 + g3 + 1.0f;
				uint16_t h = static_cast<uint16_t>(go * 64.0f);
				_heightMap[x + 16 * z] = h;
			}
		}

		for (auto& sc : out.GetSubChunks()) {
			for (auto z = 0; z < SubChunk::SIZE_Z; ++z) {
				for (auto x = 0; x < SubChunk::SIZE_X; ++x) {
					auto height = _heightMap[x + SubChunk::SIZE_X * z];
					auto pos = sc.GetPosition();
					auto dY = std::max<int>(std::min<int>(height - pos._y, SubChunk::SIZE_Y), 0);
					for (auto y = 0; y < dY; ++y) {

						BlockData* block = blockManager.GetBlock("grass");
						if (pos._y + y < height - 1)
							block = blockManager.GetBlock("dirt");
						if (pos._y + y < height - 3) {
							if (glm::simplex(
									glm::vec3((pos._x + x) * 0.125f, (pos._y + y) * 0.125f, (pos._z + z) * 0.125f))
								< -0.2f)
								continue;
							block = blockManager.GetBlock("stone");
						}
						sc.SetBlock(x, y, z, *block);
					}
				}
			}
			for (auto& b : sc.GetBlocks()) {
				if (b)
					b->Update(0);
			}
		}
	}

	void TerrainGenerator::GenerateBlock(int x, int y, int z, Block& block) {}

}