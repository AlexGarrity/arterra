#pragma once

#include "PCH.hpp"

#include "gamestate/Base.hpp"

#include "world/World.hpp"
#include "model/CullableModel.hpp"
#include "renderer/ChunkRenderer.hpp"
#include "renderer/ShaderManager.hpp"
#include "world/TerrainGenerator.hpp"
#include "block/BlockManager.hpp"
#include "texture/TextureAtlas.hpp"

namespace arterra {


	namespace gamestate {

		class Test : public Base {
		public:
			explicit Test(Engine* engine);

			void Input(float deltaTime) override;
			void Update(float deltaTime) override;
			void Render(float deltaTime) override;

		private:
			float _timeToResourceUnload = 5.0f;
			CullableModel _blockModel;
			
			TextureAtlas _atlas;
			ChunkRenderer _chunkRenderer;
			World _world;
			TerrainGenerator _terrainGenerator;
			ShaderManager _shaderManager;
			BlockManager _blockManager;

			// Temporary player controller.
			float _speed = 2.0f;
			float _rotSpeed = glm::radians(15.0f);
		};

	}

}