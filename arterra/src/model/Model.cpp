#include "model/Model.hpp"

#include "model/OBJ.hpp"

namespace arterra {

	Model::Model(std::string filepath)
		: Model()
	{
		Create(filepath);
	}


	bool Model::Create(std::string filepath)
	{
		// Load and get handle to the resource
		auto resource = ResourceManager::Get().Load(filepath);
		if (!resource) {
			Logger::Get().Log(Logger::Warning, "Failed to load model '", filepath, "'");
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
			OBJ::Load(resourceHandle._resource->_data, vertices, normals, uvs);
		}
		
		_buffers._vao.Bind();
		
		_buffers._positions.Create(vertices, 3, GL_FLOAT);
		_buffers._uvs.Create(uvs, 3, GL_FLOAT);
		_buffers._normals.Create(normals, 3, GL_FLOAT);
		
		_buffers._vao.AddBuffer(_buffers._positions);
		_buffers._vao.AddBuffer(_buffers._uvs);
		_buffers._vao.AddBuffer(_buffers._normals);

		// Get vertex count
		_vertexCount = vertices.size();
		return true;
	}


}