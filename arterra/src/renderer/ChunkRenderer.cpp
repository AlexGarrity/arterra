#include "renderer/ChunkRenderer.hpp"

namespace arterra {

	ChunkRenderer::ChunkRenderer() {
		_renderables.reserve(128);
	}

	void ChunkRenderer::AddChunk(Chunk &chunk) {
		for (auto &sc : chunk.GetSubChunks()) {
			this->AddSubChunk(sc);
		}
	}

	void ChunkRenderer::AddSubChunk(SubChunk &subChunk) {
		auto &&mesh = ChunkMesh(subChunk);
		_renderables.push_back(std::move(mesh));
	}

	void ChunkRenderer::AddMesh(ChunkMesh mesh) {
		_renderables.push_back(mesh);
	}

	void ChunkRenderer::Render() {
		for (auto &renderable : _renderables) {
			renderable.Bind();
			glDrawArrays(GL_TRIANGLES, 0, renderable.GetVertexCount());
		}
	}

}