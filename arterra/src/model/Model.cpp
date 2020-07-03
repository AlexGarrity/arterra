#include "model/Model.hpp"

namespace arterra {

	Model::Model()
	{
		glGenBuffers(1, &_buffers._vertexBuffer);
		glGenBuffers(1, &_buffers._uvBuffer);
		glGenBuffers(1, &_buffers._normalBuffer);
	}

	Model::Model(std::string filepath)
		: Model()
	{
		Create(filepath);
	}

	Model::~Model()
	{
		glDeleteBuffers(1, &_buffers._vertexBuffer);
		glDeleteBuffers(1, &_buffers._uvBuffer);
		glDeleteBuffers(1, &_buffers._normalBuffer);
	}

	bool Model::Create(std::string filepath)
	{
		// Load and get handle to the resource
		auto resource = ResourceManager::Get().Load(filepath);
		if (!resource) {
			Logger::Get().Log(Logger::Warning, "Failed to load model '", filepath, "'");
			return false;
		}
		auto resourceHandle = ResourceManager::Get().GetHandle(filepath);

		// Create vectors for storing useful things
		std::vector<float_t> vertices;
		std::vector<float_t> normals;
		std::vector<float_t> uvs;

		// Load the model. Scoped to ensure deletion
		{
			auto objModel = OBJ { resourceHandle._resource->_data };

			// Set model to match loaded model
			vertices = objModel.Vertices();
			uvs = objModel.UVs();
			normals = objModel.Normals();
		}

		// Bind vertex data
		glBindBuffer(GL_ARRAY_BUFFER, _buffers._vertexBuffer);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), vertices.data(), GL_STATIC_DRAW);

		glBindBuffer(GL_ARRAY_BUFFER, _buffers._uvBuffer);
		glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(GLfloat), uvs.data(), GL_STATIC_DRAW);

		glBindBuffer(GL_ARRAY_BUFFER, _buffers._normalBuffer);
		glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(GLfloat), normals.data(), GL_STATIC_DRAW);

		// Get vertex count and clear from memory
		_vertexCount = vertices.size();
		vertices.clear();
		normals.clear();
		uvs.clear();

		return true;
	}

	void Model::Render()
	{
		// Bind vertex coords to layout 0
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, _buffers._vertexBuffer);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

		// Bind UV coords to layout 1
		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, _buffers._uvBuffer);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);

		// Bind Normal cords to layout 2
		glEnableVertexAttribArray(2);
		glBindBuffer(GL_ARRAY_BUFFER, _buffers._normalBuffer);
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);

		// Draw
		glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(_vertexCount));

		// Disable (probably good practice)
		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(2);
	}

}