#include "model/CullableModel.hpp"

#include "model/OBJ.hpp"

namespace arterra {

	CullableModel::CullableModel(std::string filepath)
		: CullableModel()
	{
		Create(filepath);
	}

	bool CullableModel::Create(std::string filepath)
	{
		// Load and get handle to the resource
		auto resource = ResourceManager::Get().Load(filepath);
		if (!resource) {
			Logger::Get().Log(Logger::Warning, "Failed to load CullableModel '", filepath, "'");
			return false;
		}

		// Create vectors for storing useful things
		std::vector<float_t> vertices;
		std::vector<float_t> normals;
		std::vector<float_t> uvs;

		{
			// Get a handle to the resource data
			auto resourceHandle = ResourceManager::Get().GetHandle(filepath);
			// Parse the OBJ file
			OBJ::Load(resourceHandle._resource->_data, vertices, uvs, normals);
		}

		_verticesPosY.insert(_verticesPosY.end(), vertices.begin(), vertices.begin() + 9);
		_verticesPosY.insert(_verticesPosY.end(), vertices.begin() + 54, vertices.begin() + 63);
		_verticesNegY.insert(_verticesNegY.end(), vertices.begin() + 27, vertices.begin() + 36);
		_verticesNegY.insert(_verticesNegY.end(), vertices.begin() + 81, vertices.begin() + 90);

		_verticesPosX.insert(_verticesPosX.end(), vertices.begin() + 36, vertices.begin() + 45);
		_verticesPosX.insert(_verticesPosX.end(), vertices.begin() + 90, vertices.begin() + 99);
		_verticesNegX.insert(_verticesNegX.end(), vertices.begin() + 18, vertices.begin() + 27);
		_verticesNegX.insert(_verticesNegX.end(), vertices.begin() + 72, vertices.begin() + 81);

		_verticesPosZ.insert(_verticesPosZ.end(), vertices.begin() + 45, vertices.begin() + 54);
		_verticesPosZ.insert(_verticesPosZ.end(), vertices.begin() + 99, vertices.begin() + 108);
		_verticesNegZ.insert(_verticesNegZ.end(), vertices.begin() + 9, vertices.begin() + 18);
		_verticesNegZ.insert(_verticesNegZ.end(), vertices.begin() + 63, vertices.begin() + 72);

		return true;
	}

	std::vector<float_t> CullableModel::GetFace(Direction direction)
	{
		switch (direction) {
			case Direction::PosX:
				return _verticesPosX;
			case Direction::PosY:
				return _verticesPosY;
			case Direction::PosZ:
				return _verticesPosZ;
			case Direction::NegX:
				return _verticesNegX;
			case Direction::NegY:
				return _verticesNegY;
			case Direction::NegZ:
				return _verticesNegZ;
		}
		return _verticesNegX;
	}

}