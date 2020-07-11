#pragma once

#include "PCH.hpp"

#include "block/BlockData.hpp"

namespace arterra {

	class BlockManager {
	public:
		BlockManager();

		bool AddBlock(BlockData&& blockData, std::string identifier);
		bool RemoveBlock(std::string identifier);

		BlockData *GetBlock(std::string identifier);
		BlockData *GetBlock(size_t index);

	private:
		std::vector<BlockData> _blocks;
		std::unordered_map<std::string, size_t> _blockMapper;
	};

}