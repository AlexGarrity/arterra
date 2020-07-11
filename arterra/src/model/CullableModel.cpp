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

		// TODO - fix this eltritch horror of a ridiculous solution

		_posVertices[0].insert(_posVertices[0].end(), vertices.begin() + 36, vertices.begin() + 45);
		_posVertices[0].insert(_posVertices[0].end(), vertices.begin() + 90, vertices.begin() + 99);
		_posVertices[1].insert(_posVertices[1].end(), vertices.begin() + 18, vertices.begin() + 27);
		_posVertices[1].insert(_posVertices[1].end(), vertices.begin() + 72, vertices.begin() + 81);
		_posVertices[2].insert(_posVertices[2].end(), vertices.begin(), vertices.begin() + 9);
		_posVertices[2].insert(_posVertices[2].end(), vertices.begin() + 54, vertices.begin() + 63);
		_posVertices[3].insert(_posVertices[3].end(), vertices.begin() + 27, vertices.begin() + 36);
		_posVertices[3].insert(_posVertices[3].end(), vertices.begin() + 81, vertices.begin() + 90);
		_posVertices[5].insert(_posVertices[5].end(), vertices.begin() + 45, vertices.begin() + 54);
		_posVertices[5].insert(_posVertices[5].end(), vertices.begin() + 99, vertices.begin() + 108);
		_posVertices[4].insert(_posVertices[4].end(), vertices.begin() + 9, vertices.begin() + 18);
		_posVertices[4].insert(_posVertices[4].end(), vertices.begin() + 63, vertices.begin() + 72);

		_texVertices[0].insert(_texVertices[0].end(), uvs.begin() + 24, uvs.begin() + 30);
		_texVertices[0].insert(_texVertices[0].end(), uvs.begin() + 60, uvs.begin() + 66);
		_texVertices[1].insert(_texVertices[1].end(), uvs.begin() + 12, uvs.begin() + 18);
		_texVertices[1].insert(_texVertices[1].end(), uvs.begin() + 48, uvs.begin() + 54);
		_texVertices[2].insert(_texVertices[2].end(), uvs.begin() + 0, uvs.begin() + 6);
		_texVertices[2].insert(_texVertices[2].end(), uvs.begin() + 36, uvs.begin() + 42);
		_texVertices[3].insert(_texVertices[3].end(), uvs.begin() + 18, uvs.begin() + 24);
		_texVertices[3].insert(_texVertices[3].end(), uvs.begin() + 54, uvs.begin() + 60);
		_texVertices[5].insert(_texVertices[5].end(), uvs.begin() + 30, uvs.begin() + 36);
		_texVertices[5].insert(_texVertices[5].end(), uvs.begin() + 66, uvs.begin() + 72);
		_texVertices[4].insert(_texVertices[4].end(), uvs.begin() + 6, uvs.begin() + 12);
		_texVertices[4].insert(_texVertices[4].end(), uvs.begin() + 42, uvs.begin() + 48);

		return true;
	}

	std::vector<float_t> CullableModel::GetPosVertices(Direction direction) { return _posVertices[direction]; }

	std::vector<float_t> CullableModel::GetTexVertices(Direction direction) { return _texVertices[direction]; }
}