#include "model/CullableModel.hpp"

#include "model/OBJ.hpp"
#include "yaml-cpp/yaml.h"

namespace arterra {

	CullableModel::CullableModel(std::string modelName)
		: CullableModel()
	{
		Create(modelName);
	}

	bool CullableModel::Create(std::string modelName)
	{
		// Load and get handle to the model resource
		std::string modelPath = modelName + ".mobj";
		std::string configPath = modelName + ".yaml";
		auto modelResource = ResourceManager::Get().Load(modelPath);
		if (!modelResource) {
			Logger::Get().Log(Logger::Warning, "Failed to load CullableModel model'", modelName, "'");
			return false;
		}

		// Vectors for temporarily storing vertex data loaded from the model.
		std::vector<float_t> vertices;
		std::vector<float_t> normals;
		std::vector<float_t> uvs;

		{
			// Get a handle to the model resource data
			auto resourceHandle = ResourceManager::Get().GetHandle(modelPath);
			// Parse the OBJ file
			OBJ::Load(resourceHandle._resource->_data, vertices, uvs, normals);
		}

		// Parse in the yaml configuration file for the cullable model
		auto configResource = ResourceManager::Get().Load(configPath);
		if (!configResource) {
			Logger::Get().Log(Logger::Warning, "Failed to load config for CullableModel '", modelName, "'");
			return false;
		}
		
		auto dataHandle = ResourceManager::Get().GetHandle(configPath);
		std::string configSrc = std::string(dataHandle._resource->_data.begin(), dataHandle._resource->_data.end());
		YAML::Node modelConfig = YAML::Load(configSrc);
		if (!modelConfig["CullableIndicesData"]) {
			Logger::Get().Log(Logger::Warning, "The CullableModel '", modelName,
				"' config does not contain 'CullableIndicesData'");
		}

		for (YAML::const_iterator dit = modelConfig["CullableIndicesData"].begin();
			 dit != modelConfig["CullableIndicesData"].end(); dit++) {
				 
			std::string direction = dit->first.as<std::string>();

			// Convert the parsed direction value to the equivalent integer.
			int dir;
			if (direction == "PosX") {
				dir = 0;
			}else if (direction == "NegX") {
				dir = 1;
			}else if (direction == "PosY") {
				dir = 2;
			}else if (direction == "NegY") {
				dir = 3;
			}else if (direction == "PosZ") {
				dir = 4;
			}else if (direction == "NegZ") {
				dir = 5;
			}else if (direction == "Always") {
				dir = 6;
			}
			
			// Iterate through all the indices for this direction.
			for (YAML::const_iterator iit = dit->second.begin(); iit != dit->second.end(); iit++) {
				// Multiple the indices by 3 since there are 3 floats for each indice.
				int indiceGroupStart = iit->as<int>();
				iit++;
				int indiceGroupEnd = iit->as<int>();
				
				// Add the individual floats to the correct vertex array.
				_posVertices[dir].insert(_posVertices[dir].end(), vertices.begin() + (indiceGroupStart * 3),
					vertices.begin() + (indiceGroupEnd * 3) + 3);
				
				_texVertices[dir].insert(_texVertices[dir].end(), uvs.begin() + (indiceGroupStart * 2),
					uvs.begin() + (indiceGroupEnd * 2) + 2);
			}
			
		}
		
		return true;
	}

	std::vector<float_t> CullableModel::GetPosVertices(Direction dir) const { return _posVertices[dir]; }

	std::vector<float_t> CullableModel::GetTexVertices(Direction dir) const { return _texVertices[dir]; }
}