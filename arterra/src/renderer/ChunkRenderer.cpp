#include "renderer/ChunkRenderer.hpp"

#include "model/SubChunkMesh.hpp"

#include "world/Chunk.hpp"
#include "world/SubChunk.hpp"

#include "renderer/Camera.hpp"
#include "renderer/Renderer.hpp"

#include "thread/ThreadManager.hpp"

namespace arterra {

	ChunkRenderer::ChunkRenderer(const Renderer* renderer, ThreadManager* threadManager)
		: _renderer{ renderer }
		  , _threadManager{ threadManager }
	{
		_renderables.reserve(128);
		_renderables.clear();
	}

	ChunkRenderer::~ChunkRenderer()
	{
		for (const auto& renderable : _renderables) {
			renderable->Destroy();
			delete renderable;
		}
		_renderables.clear();
	}

	void ChunkRenderer::AddChunk(Chunk* chunk)
	{
		if (!chunk)
			return;
		if (_chunksPendingGeneration.find(chunk) != _chunksPendingGeneration.end())
			return;
		_chunksPendingGeneration.emplace(chunk);
		ThreadJob j([this, chunk]()
		{
			for (auto& [pos, sc] : chunk->GetSubChunks()) {
				AddSubChunk(sc);
			}
		},
			100);
		_threadManager->PushJob(j);
	}

	void ChunkRenderer::AddChunk(Chunk& chunk) { AddChunk(&chunk); }

	void ChunkRenderer::AddSubChunk(SubChunk& sc)
	{
		if (GetMesh(sc.GetPosition()))
			return;
		AddSubChunk(&sc);
	}

	void ChunkRenderer::AddSubChunk(SubChunk* sc)
	{
		auto* scm = new SubChunkMesh(*sc);
		_renderables.emplace_back(scm);
	}

	void ChunkRenderer::UpdateSubChunks(std::vector<SubChunk*>& subChunks)
	{
		for (auto& sc : subChunks) {
			auto pos = sc->GetPosition();
			DeleteMesh(pos);
			AddSubChunk(sc);
		}
	}

	void ChunkRenderer::AddMesh(SubChunkMesh* mesh) { _renderables.emplace_back(mesh); }

	void ChunkRenderer::CullRenderables(Camera& camera)
	{
		auto& frustum = camera.GetViewFrustum();
		for (auto& r : _renderables) {
			const auto& pos = r->GetPosition();
			const auto inFrustum = frustum.ChunkInFrustum(pos);
			r->SetShouldRender(inFrustum);
		}
	}

	void ChunkRenderer::DeleteRenderables(std::vector<WorldPosition>& chunks)
	{
		for (const auto& c : chunks) { DeleteMesh(c); }
	}

	void ChunkRenderer::DeleteRenderable(const WorldPosition& position)
	{
		const auto it = GetMeshIterator(position);
		if (it == _renderables.end())
			return;
		(*it)->Destroy();
		delete *it;
		_renderables.erase(it);
	}

	SubChunkMesh* ChunkRenderer::GetMesh(const WorldPosition& position)
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

	void ChunkRenderer::DeleteMesh(const WorldPosition& position)
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
		for (const auto& renderable : _renderables) {
			if (renderable->ShouldRender()) {
				renderable->Bind();
				_renderer->DrawTriangles(renderable->GetVertexCount());
			}
		}
	}

	std::vector<SubChunkMesh*>& ChunkRenderer::GetRenderables() { return _renderables; }

}
