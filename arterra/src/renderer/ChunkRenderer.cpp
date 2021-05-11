#include "renderer/ChunkRenderer.hpp"

namespace arterra {

	ChunkRenderer::ChunkRenderer(Renderer* renderer)
	{
		_renderer = renderer;
		_renderables.reserve(128);
		_renderables.clear();
	}

	ChunkRenderer::~ChunkRenderer()
	{
		for (auto& renderable : _renderables) {
			renderable->Destroy();
			delete renderable;
		}
		_renderables.clear();
	}

	void ChunkRenderer::AddChunk(Chunk* chunk)
	{
		if (!chunk)
			return;
		for (auto& sc : chunk->GetSubChunks()) {
			AddSubChunk(sc.second);
		}
	}

	void ChunkRenderer::AddChunk(Chunk& chunk)
	{
		for (auto& sc : chunk.GetSubChunks()) {
			AddSubChunk(sc.second);
		}
	}

	void ChunkRenderer::AddSubChunk(SubChunk& subChunk)
	{
		if (GetMesh(subChunk.GetPosition()))
			return;
		AddSubChunk(&subChunk);
	}

	void ChunkRenderer::AddSubChunk(SubChunk* subChunk)
	{
		auto mesh = new SubChunkMesh(*subChunk);
		_renderables.push_back(mesh);
	}

	void ChunkRenderer::UpdateSubChunks(std::vector<SubChunk*>& subChunks)
	{
		for (auto& sc : subChunks) {
			auto pos = sc->GetPosition();
			DeleteMesh(pos);
			AddSubChunk(sc);
		}
	}

	void ChunkRenderer::AddMesh(SubChunkMesh *mesh) { _renderables.emplace_back(mesh); }

	void ChunkRenderer::CullRenderables(Camera& camera)
	{
		auto frustum = camera.GetViewFrustum();
		for (auto& r : _renderables) {
			auto &pos = r->GetPosition();
			auto inFrustum = frustum.ChunkInFrustum(pos);
			r->SetShouldRender(inFrustum);
		}
	}

	void ChunkRenderer::DeleteRenderables(std::vector<WorldPosition>& chunks)
	{
		for (auto c : chunks) {
			DeleteMesh(c);
		}
	}

	void ChunkRenderer::DeleteRenderable(WorldPosition position)
	{
		auto it = GetMeshIterator(position);
		if (it == _renderables.end())
			return;
		(*it)->Destroy();
		delete *it;
		_renderables.erase(it);
	}

	SubChunkMesh* ChunkRenderer::GetMesh(WorldPosition position)
	{
		for (auto& r : _renderables) {
			auto pos = r->GetPosition();
			if (position == pos)
				return r;
		}
		return nullptr;
	}

	std::vector<SubChunkMesh*>::iterator ChunkRenderer::GetMeshIterator(WorldPosition position)
	{
		for (auto it = _renderables.begin(); it != _renderables.end(); ++it) {
			auto pos = (*it)->GetPosition();
			if (position == pos)
				return it;
		}
		return _renderables.end();
	}

	void ChunkRenderer::DeleteMesh(WorldPosition position)
	{
		auto pos = -1;
		for (auto i = 0; i < _renderables.size(); ++i) {
			auto rPos = _renderables[i]->GetPosition();
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
			if (renderable->ShouldRender()) {
				renderable->Bind();
				_renderer->DrawTriangles(renderable->GetVertexCount());
			}
		}
	}

	std::vector<SubChunkMesh*>& ChunkRenderer::GetRenderables() { return _renderables; }

}