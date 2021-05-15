#pragma once

#include "PCH.hpp"

#include "util/Direction.hpp"

namespace arterra {

	class CullableModel {

	public:
		// Default ctor
		CullableModel() = default;
		// RAII ctor
		explicit CullableModel(const std::string &modelName);

		bool Create(const std::string &filepath);

		std::vector<float> &GetPosVertices(const Direction direction);
		std::vector<float> &GetTexVertices(const Direction direction);

		std::vector<float> GetPosVertices(const Direction direction) const;
		std::vector<float> GetTexVertices(const Direction direction) const;

	private:
		// PosX, NegX, PosY, NegY, PosZ, NegZ, ???
		std::array<std::vector<float>, 7> _posVertices;
		std::array<std::vector<float>, 7> _texVertices;

		size_t _vertexCount;
	};

}