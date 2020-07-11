#pragma once

#include "PCH.hpp"

#include "glad/glad.h"

#include "renderer/VertexArray.hpp"
#include "util/Resource.hpp"

namespace arterra {

	enum Direction {
		PosX = 0,
		NegX = 1,
		PosY = 2,
		NegY = 3,
		PosZ = 4,
		NegZ = 5,
	};

	class CullableModel {

	public:
		// Default ctor
		CullableModel() = default;
		// RAII ctor
		CullableModel(std::string filepath);

		bool Create(std::string filepath);

		std::vector<float_t> GetPosVertices(Direction direction);
		std::vector<float_t> GetTexVertices(Direction direction);

	private:
		// PosX, NegX, PosY, NegY, PosZ, NegZ
		std::array<std::vector<float_t>, 6> _posVertices;
		std::array<std::vector<float_t>, 6> _texVertices;

		size_t _vertexCount;
	};

}