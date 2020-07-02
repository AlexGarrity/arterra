#pragma once

#include "PCH.hpp"

#include "glad/glad.h"

#include "model/OBJ.hpp"
#include "util/Resource.hpp"

namespace arterra {

class Model {
    struct ModelBuffers {
        GLuint _vertexBuffer = 0;
        GLuint _uvBuffer = 0;
        GLuint _normalBuffer = 0;

        ~ModelBuffers()
        {
            glDeleteBuffers(1, &_vertexBuffer);
            glDeleteBuffers(1, &_uvBuffer);
            glDeleteBuffers(1, &_normalBuffer);
        }
    };

public:
    Model()
    {
        glGenBuffers(1, &_buffers._vertexBuffer);
        glGenBuffers(1, &_buffers._uvBuffer);
        glGenBuffers(1, &_buffers._normalBuffer);
    }

    Model(std::string filepath)
    {
        Create(filepath);
    }

    ~Model()
    {
        glDeleteBuffers(1, &_buffers._vertexBuffer);
        glDeleteBuffers(1, &_buffers._uvBuffer);
        glDeleteBuffers(1, &_buffers._normalBuffer);
    }

    bool Create(std::string filepath)
    {
        // Load and get handle to the resource
        auto resource = ResourceManager::Get().Load(filepath);
        if (!resource) {
            Logger::Get().Log(Logger::Warning, "Failed to load model '", filepath, "'");
            return false;
        }
        auto resourceHandle = ResourceManager::Get().GetHandle(filepath);

        // Load the model. Scoped to ensure deletion
        {
            auto objModel = OBJ { resourceHandle._resource->_data };

            // Set model to match loaded model
            _vertices = objModel.Vertices();
            _uvs = objModel.UVs();
            _normals = objModel.Normals();
        }

        // Bind vertex data
        glBindBuffer(GL_ARRAY_BUFFER, _buffers._vertexBuffer);
        glBufferData(GL_ARRAY_BUFFER, _vertices.size() * sizeof(GLfloat), _vertices.data(), GL_STATIC_DRAW);

        glBindBuffer(GL_ARRAY_BUFFER, _buffers._uvBuffer);
        glBufferData(GL_ARRAY_BUFFER, _uvs.size() * sizeof(GLfloat), _uvs.data(), GL_STATIC_DRAW);

        glBindBuffer(GL_ARRAY_BUFFER, _buffers._normalBuffer);
        glBufferData(GL_ARRAY_BUFFER, _normals.size() * sizeof(GLfloat), _normals.data(), GL_STATIC_DRAW);

        // Get vertex count and clear from memory
        _vertexCount = _vertices.size();
        _vertices.clear();
        _normals.clear();
        _uvs.clear();

        return true;
    }

    void Render()
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

private:
    ModelBuffers _buffers;
    uint64_t _vertexCount;

    std::vector<float_t> _vertices;
    std::vector<float_t> _normals;
    std::vector<float_t> _uvs;
};

}