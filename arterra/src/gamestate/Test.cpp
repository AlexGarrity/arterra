#include "gamestate/Test.hpp"

namespace arterra {

	namespace gamestate {

		Test::Test(Engine* engine)
			: Base(engine)
		{
			_engine->GetWindow()->SetVsync(true);
			_engine->GetWindow()->SetClearColour(0.6f, 0.8f, 1.0f, 1.0f);

			// Load the basic shader and use it
			_shaderManager.LoadShader("shaders/basic.frag", "shaders/basic.vert", "basic");
			_shaderManager.UseShader("basic");

			// Create cube model.
			_cubeModel.Create("models/cube.mobj");
		
			Block default {0, 0, 0, _cubeModel, nullptr};

			for (auto z = 0; z < 8; ++z) {
				for (auto x = 0; x < 8; ++x) {
					auto chunk = _world.CreateChunk(x, z);
					_terrainGenerator.GenerateChunk(*chunk, default);
				}
			}
		}

		void Test::Input(float_t deltaTime)
		{
			Input::Update(deltaTime);

			// Close engine with [Esc]
			if (_engine->GetWindow()->IsKeyPressed(GLFW_KEY_ESCAPE)) {
				_engine->SetShouldExit(true);
			}
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
			_engine->GetRenderer()->DrawTestCube();

			
			_shaderManager.UseShader("basic");
			_shaderManager.ActiveProgram().SetUniform("fragmentTexture", 0);
			
			_chunkRenderer.Render();

			_engine->GetWindow()->Update(deltaTime);
		}

	}

}