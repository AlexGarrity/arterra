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
		PosZ = 5,
		NegZ = 4,
	};

	class CullableModel {

	public:
		// Default ctor
		CullableModel() = default;
		// RAII ctor
		CullableModel(std::string filepath);

		bool Create(std::string filepath);

		std::vector<float_t> GetFace(Direction direction);

	private:
		std::vector<float_t> _verticesPosX;
		std::vector<float_t> _verticesNegX;
		std::vector<float_t> _verticesPosY;
		std::vector<float_t> _verticesNegY;
		std::vector<float_t> _verticesPosZ;
		std::vector<float_t> _verticesNegZ;

		size_t _vertexCount;
	};

}