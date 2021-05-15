#include "gamestate/Test.hpp"

#if defined(_MSC_VER)
	#if defined(_DEBUG)
		constexpr size_t WORLD_SIZE = 6;
	#else
		constexpr size_t WORLD_SIZE = 16;
	#endif
#else
	#if defined(NDEBUG)
		constexpr size_t WORLD_SIZE = 16;
	#else
		constexpr size_t WORLD_SIZE = 6;
	#endif
#endif

#include "block/BlockData.hpp"
#include "util/Resource.hpp"

namespace arterra {

	namespace gamestate {

		Test::Test(Engine* engine)
			: Base(engine)
			, _atlas { 256, 256 }
			, _chunkRenderer { engine->GetRenderer() }
			, _world { &_terrainGenerator, &_blockManager }
			, _terrainGenerator{&_blockManager}
		{

			_engine->GetWindow()->SetVsync(true);
			_engine->GetInput()->SetLockCursor(true);
			_engine->GetWindow()->SetClearColour(0.6f, 0.8f, 1.0f, 1.0f);

			_world.ResizeLoadDistance(WORLD_SIZE);

			_engine->GetCamera()->GetTransform().Translate({0.0f, 70.0f, 0.0f});

			// ===Inputs===
			_engine->GetInput()->RegisterKeyBind("quit", sf::Keyboard::Escape);
			// Set up player control keybinds.
			_engine->GetInput()->RegisterKeyBind("forward", sf::Keyboard::W);
			_engine->GetInput()->RegisterKeyBind("backward", sf::Keyboard::S);
			_engine->GetInput()->RegisterKeyBind("left", sf::Keyboard::A);
			_engine->GetInput()->RegisterKeyBind("right", sf::Keyboard::D);
			_engine->GetInput()->RegisterKeyBind("up", sf::Keyboard::LShift);
			_engine->GetInput()->RegisterKeyBind("down", sf::Keyboard::LControl);
			_engine->GetInput()->RegisterKeyBind("log", sf::Keyboard::Tilde);
			//_engine->GetInput()->RegisterKeyBind("rot-right", sf::Keyboard::E);
			_engine->GetInput()->RegisterMouseBind("rot-left", sf::Mouse::Button::Left);
			_engine->GetInput()->RegisterMouseBind("rot-right", sf::Mouse::Button::Right);

			// ===GUI===
			std::vector<float> vPos = { 400.0f, 200.0f, 400.0f, 500.0f, 800.0f, 200.0f, 800.0f, 200.0f, 400.0f,
				500.0f, 800.0f, 500.0f };
			std::vector<float> vPos2 = { 805.0f, 200.0f, 805.0f, 500.0f, 1100.0f, 200.0f, 1100.0f, 200.0f,
				805.0f, 500.0f, 1100.0f, 500.0f };
			std::vector<float> vUV = { 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f };

			// Load the shaders.
			_shaderManager.LoadShader("shaders/basic.frag", "shaders/basic.vert", "basic");

			// Create cube model.
			_blockModel.Create("models/Block");

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

			_blockManager.AddBlock(
				BlockData { *stoneTexture, *stoneTexture, *stoneTexture, _blockModel }, "stone");
			_blockManager.AddBlock(
				BlockData { *grassTextureTop, *grassTextureSide, *dirtTexture, _blockModel }, "grass");
			_blockManager.AddBlock(BlockData { *dirtTexture, *dirtTexture, *dirtTexture, _blockModel }, "dirt");
			_blockManager.AddBlock(BlockData { *sandTexture, *sandTexture, *sandTexture, _blockModel }, "sand");
		}

		void Test::Input(const float deltaTime)
		{
			Transform& cameraTransform = _engine->GetCamera()->GetTransform();
			_speed = deltaTime * 6.0f;
			_rotSpeed = deltaTime * 30.0f;

			float aa = _engine->GetInput()->PollMouseAxis(MouseAxis::Horizontal)._delta;

			// Poll for player control inputs.
			if (_engine->GetInput()->PollKeyBind("forward")._isActive) {
				cameraTransform.Translate(cameraTransform.Forward() * _speed);
			}
			if (_engine->GetInput()->PollKeyBind("backward")._isActive) {
				cameraTransform.Translate(cameraTransform.Forward() * -_speed);
			}
			if (_engine->GetInput()->PollKeyBind("left")._isActive) {
				cameraTransform.Translate(cameraTransform.Right() * -_speed);
			}
			if (_engine->GetInput()->PollKeyBind("right")._isActive) {
				cameraTransform.Translate(cameraTransform.Right() * _speed);
			}
			if (_engine->GetInput()->PollKeyBind("up")._isActive) {
				cameraTransform.Translate(cameraTransform.Up() * _speed);
			}
			if (_engine->GetInput()->PollKeyBind("down")._isActive) {
				cameraTransform.Translate(cameraTransform.Up() * -_speed);
			}

			if (_engine->GetInput()->PollKeyBind("log")._isActive) {
				Logger::Debug("Renderable count: ", _chunkRenderer.GetRenderables().size());
			}

			float mouseX = _engine->GetInput()->PollMouseAxis(MouseAxis::Horizontal)._delta;
			float mouseY = _engine->GetInput()->PollMouseAxis(MouseAxis::Vertical)._delta;
			cameraTransform.Rotate(-_rotSpeed * -mouseY, -_rotSpeed * mouseX, 0.0f);

			// Close the window with [Esc]
			if (_engine->GetInput()->PollKeyBind("quit")._isActive) {
				_engine->SetShouldExit(true);
			}

			if (_engine->GetWindow()->ShouldClose())
				_engine->SetShouldExit(true);

		}

		void Test::Update(const float deltaTime)
		{
			_engine->GetCamera()->Update(*_engine->GetWindow(), deltaTime);

			auto wX = _engine->GetWindow()->GetWidth();
			auto wY = _engine->GetWindow()->GetHeight();

			glViewport(0, 0, wX, wY);

			_world.Update(deltaTime, _engine->GetCamera()->GetTransform().Position());

			_chunkRenderer.DeleteRenderables(_world.GetEmptyChunks());
			_chunkRenderer.UpdateSubChunks(_world.GetModifiedSubChunks());
			_chunkRenderer.CullRenderables(*_engine->GetCamera());

			// Every 5 seconds, perform garbage collection
			_timeToResourceUnload -= deltaTime;
			if (_timeToResourceUnload < 0.0f) {
				ResourceManager::Get().Unload();
				_timeToResourceUnload = 5.0f;
			}
		}

		void Test::Render(const float deltaTime)
		{
			// Clear the window
			_engine->GetWindow()->Clear();

			// Set the camera view projection so the world renders in perspective
			_shaderManager.UseShader("basic");
			_shaderManager.ActiveProgram().SetUniform("viewProjection", _engine->GetCamera()->ViewProjection());
			_shaderManager.ActiveProgram().SetUniform("fragmentColour", { 0.2f, 1.0f, 1.0f, 1.0f });

			_atlas.Bind();
			//_shaderManager.ActiveProgram().SetUniform("fragmentTexture", 0);
			_chunkRenderer.Render();

			_engine->GetWindow()->Update(deltaTime);
		}

	}

}