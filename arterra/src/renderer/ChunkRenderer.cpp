#include "renderer/ChunkRenderer.hpp"

namespace arterra {

	ChunkRenderer::ChunkRenderer(Renderer *renderer)
	{
		_renderer = renderer;
		_renderables.reserve(128);
		_renderables.clear();
	}

	ChunkRenderer::~ChunkRenderer()
	{
		for (auto& renderable : _renderables) {
			renderable.Destroy();
		}
		_renderables.clear();
	}

	void ChunkRenderer::AddChunk(Chunk* chunk)
	{
		if (!chunk)
			return;
		for (auto& sc : chunk->GetSubChunks()) {
			AddSubChunk(sc);
		}
	}

	void ChunkRenderer::AddChunk(Chunk& chunk)
	{
		for (auto& sc : chunk.GetSubChunks()) {
			AddSubChunk(sc);
		}
	}

	void ChunkRenderer::AddSubChunk(SubChunk& subChunk)
	{
		if (GetChunkMesh(subChunk.GetPosition()))
			return;
		AddSubChunk(&subChunk);
	}

	void ChunkRenderer::AddSubChunk(SubChunk* subChunk)
	{
		auto&& mesh = ChunkMesh(*subChunk);
		_renderables.push_back(std::move(mesh));
	}

	void ChunkRenderer::UpdateSubChunks(std::vector<SubChunk*>& subChunks)
	{
		for (auto& sc : subChunks) {
			auto pos = sc->GetPosition();
			DeleteMesh(pos);
			AddSubChunk(sc);
		}
	}

	void ChunkRenderer::AddMesh(ChunkMesh mesh) { _renderables.push_back(mesh); }

	void ChunkRenderer::CullRenderables(Camera &camera) {
		auto frustum = camera.GetViewFrustum();
		for (auto &r : _renderables) {
			r.SetShouldRender(frustum.ChunkInFrustum(r.GetPosition()));
		}
	}

	ChunkMesh* ChunkRenderer::GetChunkMesh(BlockPosition position)
	{
		for (auto& r : _renderables) {
			auto pos = r.GetPosition();
			if (position == pos)
				return &r;
		}
		return nullptr;
	}

	void ChunkRenderer::DeleteMesh(BlockPosition position)
	{
		auto pos = -1;
		for (auto i = 0; i < _renderables.size(); ++i) {
			auto rPos = _renderables[i].GetPosition();
			if (position == rPos) {
				pos = i;
				break;
			}
		}
		if (pos != -1)
			_renderables.erase(_renderables.begin() + pos);
	}

	void ChunkRenderer::Render()
	{
		for (auto& renderable : _renderables) {
			if (renderable.ShouldRender()) {
				renderable.Bind();
				_renderer->DrawTriangles(renderable.GetVertexCount());
				glDrawArrays(GL_TRIANGLES, 0, renderable.GetVertexCount());
			}		
		}
	}

	std::vector<ChunkMesh>& ChunkRenderer::GetRenderables() { return _renderables; }

}