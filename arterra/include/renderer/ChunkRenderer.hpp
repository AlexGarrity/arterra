#pragma once

#include "PCH.hpp"
namespace arterra {
	
	class ThreadManager;
	class Renderer;
	class Chunk;
	class SubChunk;
	class SubChunkMesh;
	class Camera;
	struct WorldPosition;
	
	class ChunkRenderer {
	public:
		ChunkRenderer(const Renderer* renderer, ThreadManager *threadManager);
		~ChunkRenderer();

		void AddChunk(Chunk* chunk);
		void AddChunk(Chunk& chunk);
		void AddSubChunk(SubChunk* sc);
		void AddSubChunk(SubChunk& sc);
		void AddMesh(SubChunkMesh *mesh);

		void CullRenderables(Camera &camera);
		void DeleteRenderables(std::vector<WorldPosition> &chunks);
		void DeleteRenderable(const WorldPosition &pos);

		void UpdateSubChunks(std::vector<SubChunk*>& subChunks);
		SubChunkMesh* GetMesh(const WorldPosition &position);
		std::vector<SubChunkMesh*>::iterator GetMeshIterator(WorldPosition position);
		void DeleteMesh(const WorldPosition &position);

		std::vector<SubChunkMesh*>& GetRenderables();

		void Render();

	private:
		std::vector<SubChunkMesh*> _renderables;
		std::unordered_set<Chunk*> _chunksPendingGeneration;
		
		const Renderer *_renderer;
		ThreadManager* _threadManager;
	};

}