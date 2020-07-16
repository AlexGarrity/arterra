#include "gamestate/Test.hpp"

namespace arterra {

	namespace gamestate {

		Test::Test(Engine* engine)
			: Base(engine)
			, _atlas { 256, 256 }
			, _chunkRenderer{engine->GetRenderer()}
			, _guiRenderer{engine->GetRenderer()}
		{
			
			_engine->GetWindow()->SetVsync(true);
			_engine->GetWindow()->SetLockCursor(true);
			_engine->GetWindow()->SetClearColour(0.6f, 0.8f, 1.0f, 1.0f);
			
			// ===Inputs===
			_engine->GetInput()->RegisterKeyBind("quit", sf::Keyboard::Escape);
			// Set up player control keybinds.
			_engine->GetInput()->RegisterKeyBind("forward", sf::Keyboard::W);
			_engine->GetInput()->RegisterKeyBind("backward", sf::Keyboard::S);
			_engine->GetInput()->RegisterKeyBind("left", sf::Keyboard::A);
			_engine->GetInput()->RegisterKeyBind("right", sf::Keyboard::D);
			_engine->GetInput()->RegisterKeyBind("up", sf::Keyboard::LShift);
			_engine->GetInput()->RegisterKeyBind("down", sf::Keyboard::LControl);
			//_engine->GetInput()->RegisterKeyBind("rot-left", sf::Keyboard::Q);
			//_engine->GetInput()->RegisterKeyBind("rot-right", sf::Keyboard::E);
			_engine->GetInput()->RegisterMouseBind("rot-left", sf::Mouse::Button::Left);
			_engine->GetInput()->RegisterMouseBind("rot-right", sf::Mouse::Button::Right);
			
			// ===GUI===
			_element1 = UIElement { 400, 300, glm::vec2(100.0f, 100.0f), UIElementAnchor::BottomLeft};
			
			_element1.GetGUIElement()->SetShouldRender(true);
			_guiRenderer.AddElement(_element1.GetGUIElement());
			
			_guiTexture.Load("textures/gui.png");
			
			// Load the shaders.
			_shaderManager.LoadShader("shaders/basic.frag", "shaders/basic.vert", "basic");
			_shaderManager.LoadShader("shaders/gui_sprite_simple.frag", "shaders/gui_sprite_simple.vert", "gui");
			_shaderManager.LoadShader("shaders/gui_sprite_spliced.frag", "shaders/gui_sprite_spliced.vert", "gui-fancy");

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

			_blockManager.AddBlock(BlockData { *stoneTexture, *stoneTexture, *stoneTexture, _blockModel }, "stone");
			_blockManager.AddBlock(
				BlockData { *grassTextureTop, *grassTextureSide, *dirtTexture, _blockModel }, "grass");
			_blockManager.AddBlock(BlockData { *dirtTexture, *dirtTexture, *dirtTexture, _blockModel }, "dirt");
			_blockManager.AddBlock(BlockData { *sandTexture, *sandTexture, *sandTexture, _blockModel }, "sand");

			for (auto z = -2; z < 2; ++z) {
				for (auto x = -2; x < 2; ++x) {
					auto chunk = _world.CreateChunkCS(x, z);
					_terrainGenerator.GenerateChunk(*chunk, _blockManager);
				}
			}
		}

		void Test::Input(float_t deltaTime)
		{
			Transform& cameraTransform = _engine->GetCamera()->GetTransform();
			_speed = deltaTime * 16.0f;
			_rotSpeed = deltaTime * 3.0f;
			
			float_t aa = _engine->GetInput()->PollMouseAxis(MouseAxis::Horizontal)._delta;
			
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
			float_t mouseX = _engine->GetInput()->PollMouseAxis(MouseAxis::Horizontal)._delta;
			float_t mouseY = _engine->GetInput()->PollMouseAxis(MouseAxis::Vertical)._delta;
			cameraTransform.Rotate(-_rotSpeed * -mouseY, -_rotSpeed * mouseX, 0.0f);
			
			// Close the window with [Esc]
			if (_engine->GetInput()->PollKeyBind("quit")._isActive) {
				_engine->SetShouldExit(true);
			}
			
			if (_engine->GetWindow()->ShouldClose()) _engine->SetShouldExit(true);
		}

		void Test::Update(float_t deltaTime)
		{
			_engine->GetCamera()->Update(*_engine->GetWindow(), deltaTime);

			
			auto wX = _engine->GetWindow()->GetWidth();
			auto wY = _engine->GetWindow()->GetHeight();

			glViewport(0, 0, wX, wY);

			_world.Update(deltaTime);
			_chunkRenderer.UpdateSubChunks(_world.GetModifiedSubChunks());
			_chunkRenderer.CullRenderables(*_engine->GetCamera());
			// TODO: guirenderer.updateelements(uimanager.getmodifiedelements)

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
			
			// Set the camera view projection so the world renders in perspective
			_shaderManager.UseShader("basic");
			_shaderManager.ActiveProgram().SetUniform("viewProjection", _engine->GetCamera()->ViewProjection());
			_shaderManager.ActiveProgram().SetUniform("fragmentColour", { 0.2f, 1.0f, 1.0f, 1.0f });
			
			_atlas.Bind();
			//_shaderManager.ActiveProgram().SetUniform("fragmentTexture", 0);
			_chunkRenderer.Render();
			
			_shaderManager.UseShader("gui-fancy");
			_guiTexture.Bind();
			_shaderManager.ActiveProgram().SetUniform("u_ColourTint", glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
			_shaderManager.ActiveProgram().SetUniform("viewProjection", _engine->GetCamera()->GuiProjection());
			//_shaderManager.ActiveProgram().SetUniform("u_Pixelborder", glm::vec2(0.01f, 0.01333f));
			_shaderManager.ActiveProgram().SetUniform("u_Pixelborder", glm::vec2(0.1f, 0.1333f));
			_shaderManager.ActiveProgram().SetUniform("u_Textureborder", 0.3125f);
			_shaderManager.ActiveProgram().SetUniform("u_DebugMode", 0);
			_guiRenderer.Render();

			_engine->GetWindow()->Update(deltaTime);
		}

	}

}