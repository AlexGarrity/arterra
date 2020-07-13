#include "gamestate/Test.hpp"

namespace arterra {

	namespace gamestate {

		Test::Test(Engine* engine)
			: Base(engine)
			, _atlas { 256, 256 }
			, _chunkRenderer{engine->GetRenderer()}
		{
			_engine->GetWindow()->SetVsync(true);
			_engine->GetWindow()->SetClearColour(0.6f, 0.8f, 1.0f, 1.0f);

			// Load the basic shader and use it
			_shaderManager.LoadShader("shaders/basic.frag", "shaders/basic.vert", "basic");
			_shaderManager.UseShader("basic");

			// Create cube model.
			_cubeModel.Create("models/cube.mobj");

			_atlas.LoadTexture("textures/stone.png", "stone");
			_atlas.LoadTexture("textures/grass_top.png", "grass_top");
			_atlas.LoadTexture("textures/grass_side.png", "grass_side");
			_atlas.LoadTexture("textures/sand.png", "sand");
			_atlas.LoadTexture("textures/dirt.png", "dirt");

			auto stoneTexture = _atlas.GetTexture("stone");
			auto grassTextureTop = _atlas.GetTexture("grass_top");
			auto grassTextureSide = _atlas.GetTexture("grass_side");
			auto sandTexture = _atlas.GetTexture("sand");
			auto dirtTexture = _atlas.GetTexture("dirt");

			_blockManager.AddBlock(BlockData { *stoneTexture, *stoneTexture, *stoneTexture, _cubeModel }, "stone");
			_blockManager.AddBlock(
				BlockData { *grassTextureTop, *grassTextureSide, *dirtTexture, _cubeModel }, "grass");
			_blockManager.AddBlock(BlockData { *dirtTexture, *dirtTexture, *dirtTexture, _cubeModel }, "dirt");
			_blockManager.AddBlock(BlockData { *sandTexture, *sandTexture, *sandTexture, _cubeModel }, "sand");

			for (auto z = -12; z < 12; ++z) {
				for (auto x = -12; x < 12; ++x) {
					auto chunk = _world.CreateChunk(x, z);
					_terrainGenerator.GenerateChunk(*chunk, _blockManager);
				}
			}
		}

		void Test::Input(float_t deltaTime)
		{
			if (_engine->GetWindow()->ShouldClose()) _engine->SetShouldExit(true);
		}

		void Test::Update(float_t deltaTime)
		{
			_engine->GetCamera()->Update(*_engine->GetWindow(), deltaTime);

			// Set the camera view projection so the world renders in perspective
			_shaderManager.ActiveProgram().SetUniform("viewProjection", _engine->GetCamera()->ViewProjection());
			_shaderManager.ActiveProgram().SetUniform("fragmentColour", { 0.2f, 1.0f, 1.0f, 1.0f });

			auto wX = _engine->GetWindow()->GetWidth();
			auto wY = _engine->GetWindow()->GetHeight();

			glViewport(0, 0, wX, wY);

			_world.Update(deltaTime);
			_chunkRenderer.UpdateSubChunks(_world.GetModifiedSubChunks());
			_chunkRenderer.CullRenderables(*_engine->GetCamera());

			// Every 5 seconds, perform garbage collection
			_timeToResourceUnload -= deltaTime;
			if (_timeToResourceUnload < 0.0f) {
				ResourceManager::Get().Unload();
				_timeToResourceUnload = 5.0f;
			}
		}

		void Test::Render(float_t deltaTime)
		{
			// Clear the window
			_engine->GetWindow()->Clear();

			_shaderManager.UseShader("basic");
			_atlas.Bind();
			//_shaderManager.ActiveProgram().SetUniform("fragmentTexture", 0);

			_chunkRenderer.Render();

			_engine->GetWindow()->Update(deltaTime);
		}

	}

}