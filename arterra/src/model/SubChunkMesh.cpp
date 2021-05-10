#include "model/SubChunkMesh.hpp"

namespace arterra {

	SubChunkMesh::SubChunkMesh() {}

	SubChunkMesh::SubChunkMesh(SubChunk& subChunk)
		: _position { subChunk.GetPosition() }
	{
		AddSubChunk(subChunk);
		GenerateMesh();
	}

	SubChunkMesh::SubChunkMesh(const SubChunkMesh& other)
	{
		_posVertices = std::move(other._posVertices);
		_uvVertices = std::move(other._uvVertices);
		_vertexArray = other._vertexArray;
		_posBuffer = other._posBuffer;
		_texBuffer = other._texBuffer;
		_position = other._position;
	}

	SubChunkMesh::~SubChunkMesh() {}

	void SubChunkMesh::Destroy()
	{
		_vertexArray.Destroy();
		_posBuffer.Destroy();
		_texBuffer.Destroy();
	}

	void SubChunkMesh::AddChunk() {}

	void SubChunkMesh::AddSubChunk(SubChunk& subChunk)
	{
		for (auto& block : subChunk.GetBlocks()) {
			if (!block)
				continue;

			AddBlock(*block);
		}
	}

	void SubChunkMesh::AddBlock(Block& block)
	{
		if (!block.IsVisible())
			return;
		auto visibleFaces = block.GetVisibleFaces();
		for (auto i = 0; i < visibleFaces.size()+1; ++i) {
			if (i != 6) {
				if (visibleFaces[i] == false)
					continue;
			}
			
			auto blockData = block.GetData();
			auto posVertices = blockData.GetModel().GetPosVertices(Direction(i));
			auto texVertices = blockData.GetModel().GetTexVertices(Direction(i));
			auto texture = blockData.GetTexture(Direction(i));

			auto position = block.GetPosition();
			for (size_t j = 0; j < posVertices.size() / 3; ++j) {
				auto iPos = j * 3;
				posVertices[iPos + 0] += position._x;
				posVertices[iPos + 1] += position._y;
				posVertices[iPos + 2] += position._z;

				auto iTex = j * 2;
				texVertices[iTex + 0] *= texture._width;
				texVertices[iTex + 0] += texture._x;
				texVertices[iTex + 1] *= texture._height;
				texVertices[iTex + 1] += texture._y;
			}
			AddFace(posVertices, texVertices);
		}
	}

	void SubChunkMesh::AddFace(std::vector<float_t> posVertices, std::vector<float_t> texVertices)
	{
		_posVertices.insert(_posVertices.end(), posVertices.begin(), posVertices.end());
		_uvVertices.insert(_uvVertices.end(), texVertices.begin(), texVertices.end());
	}

	void SubChunkMesh::GenerateMesh()
	{
		_posBuffer.Create(_posVertices, 3, GL_FLOAT);
		_texBuffer.Create(_uvVertices, 2, GL_FLOAT);
		_vertexArray.AddBuffer(_posBuffer);
		_vertexArray.AddBuffer(_texBuffer);
		_posVertices.clear();
		_uvVertices.clear();
	}

	void SubChunkMesh::Bind() { _vertexArray.Bind(); }

	GLuint SubChunkMesh::GetVertexCount() const { return _vertexArray.GetVertexCount(); }

}