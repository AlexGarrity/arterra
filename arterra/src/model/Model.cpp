#include "model/Model.hpp"

#include "model/OBJ.hpp"
#include "util/Resource.hpp"

namespace arterra {

	Model::Model(const std::string &filepath)
		: Model()
	{
		Create(filepath);
	}

	bool Model::Create(const std::string &filepath)
	{
		// Load and get handle to the resource
		auto resource = ResourceManager::Get().Load(filepath);
		if (!resource) {
			Logger::Warning("Failed to load model '", filepath, "'");
			return false;
		}

		// Create vectors for storing useful things
		std::vector<float> vertices;
		std::vector<float> normals;
		std::vector<float> uvs;

		{
			// Get a handle to the resource data
			auto resourceHandle = ResourceManager::Get().GetHandle(filepath);
			// Parse the OBJ file
			OBJ::Load(resourceHandle._resource->_data, vertices, uvs, normals);
		}

		_buffers._vao.Bind();

		_buffers._positions.Create(vertices, 3, GL_FLOAT);
		_buffers._uvs.Create(uvs, 2, GL_FLOAT);
		_buffers._normals.Create(normals, 3, GL_FLOAT);

		_buffers._vao.AddBuffer(_buffers._positions);
		_buffers._vao.AddBuffer(_buffers._uvs);
		_buffers._vao.AddBuffer(_buffers._normals);

		// Get vertex count
		_vertexCount = vertices.size();
		return true;
	}

	void Model::Bind() const { _buffers._vao.Bind(); }
}
