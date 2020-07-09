#include "model/ChunkMesh.hpp"

namespace arterra {

	ChunkMesh::ChunkMesh() {}

	ChunkMesh::ChunkMesh(SubChunk& subChunk)
		: _position { subChunk.GetPosition() }
	{
		AddSubChunk(subChunk);
		GenerateMesh();
	}

	ChunkMesh::ChunkMesh(const ChunkMesh& other)
	{
		_vertices = std::move(other._vertices);
		_vertexArray = other._vertexArray;
		_vertexBuffer = other._vertexBuffer;
		_position = other._position;
	}

	ChunkMesh::~ChunkMesh() {}

	void ChunkMesh::Destroy()
	{
		_vertexArray.Destroy();
		_vertexBuffer.Destroy();
	}

	void ChunkMesh::AddChunk() {}

	void ChunkMesh::AddSubChunk(SubChunk& subChunk)
	{
		for (auto block : subChunk.GetBlocks()) {
			if (!block)
				continue;

			AddBlock(*block);
		}
	}

	void ChunkMesh::AddBlock(Block& block)
	{
		if (!block.IsVisible())
			return;
		auto visibleFaces = block.GetVisibleFaces();
		for (auto i = 0; i < visibleFaces.size(); ++i) {
			if (visibleFaces[i] == false)
				continue;
			auto vertices = block.GetModel().GetFace(Direction(i));
			auto position = block.GetPosition();
			for (auto j = 0; j < vertices.size(); j += 3) {
				vertices[j + 0] += position._x;
				vertices[j + 1] += position._y;
				vertices[j + 2] += position._z;
			}
			AddFace(vertices);
		}
	}

	void ChunkMesh::AddFace(std::vector<float_t> vertices)
	{
		_vertices.insert(_vertices.end(), vertices.begin(), vertices.end());
	}

	void ChunkMesh::GenerateMesh()
	{
		_vertexBuffer.Create(_vertices, 3, GL_FLOAT);
		_vertexArray.AddBuffer(_vertexBuffer);
		_vertices.clear();
	}

	void ChunkMesh::Bind() { _vertexArray.Bind(); }

	GLuint ChunkMesh::GetVertexCount() { return _vertexArray.GetVertexCount(); }

	BlockPosition ChunkMesh::GetPosition() { return _position; }

}