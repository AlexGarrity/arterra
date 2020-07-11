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
		_posVertices = std::move(other._posVertices);
		_uvVertices = std::move(other._uvVertices);
		_vertexArray = other._vertexArray;
		_posBuffer = other._posBuffer;
		_texBuffer = other._texBuffer;
		_position = other._position;
	}

	ChunkMesh::~ChunkMesh() {}

	void ChunkMesh::Destroy()
	{
		_vertexArray.Destroy();
		_posBuffer.Destroy();
		_texBuffer.Destroy();
	}

	void ChunkMesh::AddChunk() {}

	void ChunkMesh::AddSubChunk(SubChunk& subChunk)
	{
		for (auto& block : subChunk.GetBlocks()) {
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
			auto blockData = block.GetData();
			auto posVertices = blockData.GetModel().GetPosVertices(Direction(i));
			auto texVertices = blockData.GetModel().GetTexVertices(Direction(i));
			auto texture = blockData.GetTexture(Direction(i));

			auto position = block.GetPosition();
			for (auto j = 0; j < posVertices.size() / 3; ++j) {
				posVertices[(j * 3) + 0] += position._x;
				posVertices[(j * 3) + 1] += position._y;
				posVertices[(j * 3) + 2] += position._z;
				texVertices[(j * 2) + 0] *= texture._width;
				texVertices[(j * 2) + 0] += texture._x;
				texVertices[(j * 2) + 1] *= texture._height;
				texVertices[(j * 2) + 1] += texture._y;
			}
			AddFace(posVertices, texVertices);
		}
	}

	void ChunkMesh::AddFace(std::vector<float_t> posVertices, std::vector<float_t> texVertices)
	{
		_posVertices.insert(_posVertices.end(), posVertices.begin(), posVertices.end());
		_uvVertices.insert(_uvVertices.end(), texVertices.begin(), texVertices.end());
	}

	void ChunkMesh::GenerateMesh()
	{
		_posBuffer.Create(_posVertices, 3, GL_FLOAT);
		_texBuffer.Create(_uvVertices, 2, GL_FLOAT);
		_vertexArray.AddBuffer(_posBuffer);
		_vertexArray.AddBuffer(_texBuffer);
		_posVertices.clear();
		_uvVertices.clear();
	}

	void ChunkMesh::Bind() { _vertexArray.Bind(); }

	GLuint ChunkMesh::GetVertexCount() { return _vertexArray.GetVertexCount(); }

	BlockPosition ChunkMesh::GetPosition() { return _position; }

}