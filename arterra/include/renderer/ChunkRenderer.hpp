#pragma once

#include "PCH.hpp"

#include "model/SubChunkMesh.hpp"
#include "world/Chunk.hpp"
#include "world/SubChunk.hpp"
#include "renderer/Camera.hpp"
#include "renderer/Renderer.hpp"

namespace arterra {

	class ChunkRenderer {
	public:
		ChunkRenderer(Renderer *renderer);
		~ChunkRenderer();

		void AddChunk(Chunk* chunk);
		void AddChunk(Chunk& chunk);
		void AddSubChunk(SubChunk* subChunk);
		void AddSubChunk(SubChunk& subChunk);
		void AddMesh(SubChunkMesh *mesh);

		void CullRenderables(Camera &camera);
		void DeleteRenderables(std::vector<WorldPosition> &chunks);
		void DeleteRenderable(WorldPosition pos);

		void UpdateSubChunks(std::vector<SubChunk*>& subChunks);
		SubChunkMesh* GetMesh(WorldPosition position);
		std::vector<SubChunkMesh*>::iterator GetMeshIterator(WorldPosition position);
		void DeleteMesh(WorldPosition position);

		void Render();

		std::vector<SubChunkMesh*>& GetRenderables();

	private:
		std::vector<SubChunkMesh*> _renderables;
		Renderer *_renderer;
	};

}