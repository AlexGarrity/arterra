#include "gamestate/Test.hpp"

namespace arterra {

	namespace gamestate {

		Test::Test(Engine* engine)
			: Base(engine)
		{
			_engine->GetWindow()->SetVsync(true);
			_engine->GetWindow()->SetClearColour(1.0f, 0.0f, 1.0f, 1.0f);

			// Load the basic shader and use it
			_shaderManager.LoadShader("shaders/basic.frag", "shaders/basic.vert", "basic");
			_shaderManager.UseShader("basic");

			// Create cube model.
			_cubeModel.Create("models/cube.mobj");

			auto cA = _world.CreateChunk(0, 0, _cubeModel);
			auto cB = _world.CreateChunk(0, 1, _cubeModel);

			_chunkRenderer.AddChunk(cA);
			_chunkRenderer.AddChunk(cB);
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