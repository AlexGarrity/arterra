#pragma once

#include "PCH.hpp"
namespace arterra {

	class Renderer;
	class Chunk;
	class SubChunk;
	class SubChunkMesh;
	class Camera;
	struct WorldPosition;
	
	class ChunkRenderer {
	public:
		explicit ChunkRenderer(const Renderer* renderer);
		~ChunkRenderer();

		void AddChunk(Chunk* chunk);
		void AddChunk(Chunk& chunk);
		void AddSubChunk(SubChunk* subChunk);
		void AddSubChunk(SubChunk& subChunk);
		void AddMesh(SubChunkMesh *mesh);

		void CullRenderables(Camera &camera);
		void DeleteRenderables(std::vector<WorldPosition> &chunks);
		void DeleteRenderable(const WorldPosition &pos);

		void UpdateSubChunks(std::vector<SubChunk*>& subChunks);
		SubChunkMesh* GetMesh(const WorldPosition &position);
		std::vector<SubChunkMesh*>::iterator GetMeshIterator(WorldPosition position);
		void DeleteMesh(const WorldPosition &position);

		void Render();

		std::vector<SubChunkMesh*>& GetRenderables();

	private:
		std::vector<SubChunkMesh*> _renderables;
		const Renderer *_renderer;
	};

}