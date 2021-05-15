#pragma once

#include "PCH.hpp"

namespace arterra {
	class OBJ {
	public:
		using fVec = std::vector<float>;
		static bool Load(const std::vector<uint8_t> &data, fVec& vertOut, fVec& uvOut, fVec& normOut);
	};
}