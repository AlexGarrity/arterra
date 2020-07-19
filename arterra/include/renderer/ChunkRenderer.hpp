#pragma once

#include "PCH.hpp"

#include "model/ChunkMesh.hpp"
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
		void AddMesh(ChunkMesh mesh);

		void CullRenderables(Camera &camera);
		void DeleteChunks(std::vector<WorldPosition> &chunks);

		void UpdateSubChunks(std::vector<SubChunk*>& subChunks);
		ChunkMesh* GetChunkMesh(WorldPosition position);
		void DeleteMesh(WorldPosition position);

		void Render();

		std::vector<ChunkMesh>& GetRenderables();

	private:
		std::vector<ChunkMesh> _renderables;
		Renderer *_renderer;
	};

}