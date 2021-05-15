#include "model/SubChunkMesh.hpp"

#include <glad/glad.h>

#include "texture/TextureAtlas.hpp"
#include "texture/AtlasTexture.hpp"
#include "world/SubChunk.hpp"
#include "model/CullableModel.hpp"

namespace arterra {

	SubChunkMesh::SubChunkMesh() {}

	SubChunkMesh::SubChunkMesh(SubChunk& subChunk)
		: _position{ subChunk.GetPosition() }
	{
		_posVertices.reserve(ESTIMATED_POS_SIZE);
		_uvVertices.reserve(ESTIMATED_UV_SIZE);

		_pv.reserve(64);
		_tv.reserve(64);

		AddSubChunk(subChunk);
		GenerateMesh();
	}

	SubChunkMesh::SubChunkMesh(const SubChunkMesh& other)
	{
		_posVertices = other._posVertices;
		_uvVertices = other._uvVertices;
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
			if (!block->IsVisible())
				continue;

			AddBlock(*block);
		}
	}

	void SubChunkMesh::AddBlock(Block& block)
	{
		const auto visibleFaces = block.GetVisibleFaces();
		for (auto i = 0; i < visibleFaces.size() + 1; ++i) {
			if (i != 6) {
				if (visibleFaces[i] == false)
					continue;
			}

			const auto& blockData = block.GetData();
			const auto& texture = blockData.GetTexture(static_cast<Direction>(i));

			auto& model = blockData.GetModel();
			auto posVertices = model.GetPosVertices(static_cast<Direction>(i));
			auto texVertices = model.GetTexVertices(static_cast<Direction>(i));

			_pv.resize(posVertices.size());
			_tv.resize(texVertices.size());

			const auto position = block.GetPosition();
			for (size_t j = 0; j < posVertices.size() / 3; ++j) {
				const auto iPos = j * 3;
				_pv[iPos + 0] = position._x + posVertices[iPos + 0];
				_pv[iPos + 1] = position._y + posVertices[iPos + 1];
				_pv[iPos + 2] = position._z + posVertices[iPos + 2];

				const auto iTex = j * 2;
				_tv[iTex + 0] = texVertices[iTex + 0] * texture._width;
				_tv[iTex + 0] += texture._x;
				_tv[iTex + 1] = texVertices[iTex + 1] * texture._height;
				_tv[iTex + 1] += texture._y;
			}
			AddFace(_pv, _tv);
		}
	}

	void SubChunkMesh::AddFace(std::vector<float>& posVertices, std::vector<float>& texVertices)
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
		_pv.clear();
		_tv.clear();
	}

	void SubChunkMesh::SetShouldRender(bool shouldRender) { _shouldRender = shouldRender; }

	bool SubChunkMesh::ShouldRender() const { return _shouldRender; }

	void SubChunkMesh::Bind() { _vertexArray.Bind(); }

	GLuint SubChunkMesh::GetVertexCount() const { return _vertexArray.GetVertexCount(); }

	const WorldPosition& SubChunkMesh::GetPosition() const { return _position; }

}
