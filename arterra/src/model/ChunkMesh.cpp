#include "model/ChunkMesh.hpp"


namespace arterra {

	ChunkMesh::ChunkMesh() {



	}

	void ChunkMesh::AddChunk() {

	}

	void ChunkMesh::AddSubChunk(SubChunk &subChunk) {
		for (auto block : subChunk.GetBlocks()) {
			if (!block) continue;

			AddBlock(*block);
		}
	}

	void ChunkMesh::AddBlock(Block &block) {
		auto visibleFaces = block.GetVisibleFaces();
		for (auto i = 0; i < visibleFaces.size(); ++i) {
			if (visibleFaces[i] == false) continue;
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

	void ChunkMesh::AddFace(std::vector<float_t> vertices) {
		_vertices.insert(_vertices.end(), vertices.begin(), vertices.end());
	}

	void ChunkMesh::GenerateMesh() {
		_vertexBuffer.Create(_vertices, 3, GL_FLOAT);
		_vertexArray.AddBuffer(_vertexBuffer);
	}

	void ChunkMesh::Bind() {
		_vertexArray.Bind();
	}

	GLuint ChunkMesh::GetVertexCount() {
		return _vertexArray.GetVertexCount();
	}

}