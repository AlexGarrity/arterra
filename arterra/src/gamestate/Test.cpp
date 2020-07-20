#include "gamestate/Test.hpp"

namespace arterra {

	namespace gamestate {

		Test::Test(Engine* engine)
			: Base(engine)
			, _atlas { 256, 256 }
			, _guiAtlas { 256, 256 }
			, _chunkRenderer{engine->GetRenderer()}
			, _uiManager { &_shaderManager, _engine->GetRenderer(), _engine->GetWindow()->GetEvent(),
				_engine->GetInput() }
		{
			
			_engine->GetWindow()->SetVsync(true);
			_engine->GetWindow()->SetLockCursor(false);
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
			
			_engine->GetInput()->RegisterMouseBind("primary-button", sf::Mouse::Button::Left);
			
			// ===GUI===
			_guiAtlas.LoadTexture("textures/gui.png", "gui");
			_guiTexture = _guiAtlas.GetTexture("gui");

			UI::ShaderParameter p1 ( "u_ColourTint", glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), UI::ShaderParameter::Type::Vec4 );
			UI::ShaderParameter p2 ( "u_ViewProjection", _engine->GetCamera()->GuiProjection(), UI::ShaderParameter::Type::Mat4 );
			UI::ShaderParameter p3 ( "u_Pixelborder", glm::vec2(0.1f, 0.13333f), UI::ShaderParameter::Type::Vec2 );
			UI::ShaderParameter p4 ( "u_Textureborder", 0.3125f, UI::ShaderParameter::Type::Float );
			UI::ShaderParameter p5 ( "u_DebugMode", 0, UI::ShaderParameter::Type::Int );
			UI::ShaderParameter p6 ( "u_TextureCoords", glm::vec4(_guiTexture->_x, _guiTexture->_y,
					_guiTexture->_width, _guiTexture->_height), UI::ShaderParameter::Type::Vec4 );
			
			_mat1 = UI::Material { "gui-fancy" };
			_mat1.AddParameter(p1);
			_mat1.AddParameter(p2);
			_mat1.AddParameter(p3);
			_mat1.AddParameter(p4);
			_mat1.AddParameter(p5);
			_mat1.AddParameter(p6);
			
			_element1 = UI::Element { glm::vec2(300.0f, 300.0f), 400, 300, 0.0f, UI::Pivot::Centre,
				UI::Anchor::Left, nullptr, _guiTexture, _mat1 };
			_uiManager.CreateElement("el1", _element1);
			
			
			UI::ShaderParameter pp1 ( "u_ColourTint", glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), UI::ShaderParameter::Type::Vec4 );
			UI::ShaderParameter pp2 ( "u_ViewProjection", _engine->GetCamera()->GuiProjection(), UI::ShaderParameter::Type::Mat4 );
			UI::ShaderParameter pp3 ( "u_Pixelborder", glm::vec2(0.1111f, 0.1111f), UI::ShaderParameter::Type::Vec2 );
			UI::ShaderParameter pp4 ( "u_Textureborder", 0.3125f, UI::ShaderParameter::Type::Float );
			UI::ShaderParameter pp5 ( "u_DebugMode", 0, UI::ShaderParameter::Type::Int );
			UI::ShaderParameter pp6 ( "u_TextureCoords", glm::vec4(_guiTexture->_x, _guiTexture->_y,
					_guiTexture->_width, _guiTexture->_height), UI::ShaderParameter::Type::Vec4 );
			
			_mat2 = UI::Material { "gui-fancy" };
			_mat2.AddParameter(pp1);
			_mat2.AddParameter(pp2);
			_mat2.AddParameter(pp3);
			_mat2.AddParameter(pp4);
			_mat2.AddParameter(pp5);
			_mat2.AddParameter(pp6);
			
			_element2 = UI::Element { glm::vec2(1240.0f, 680.0f), 200, 200, 0.0f, UI::Pivot::BottomLeft,
				UI::Anchor::TopRight, nullptr, _guiTexture, _mat2 };
			//_uiManager.CreateElement("el2", _element2);
			
			
			UI::ShaderParameter ppp1 ( "u_ColourTint", glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), UI::ShaderParameter::Type::Vec4 );
			UI::ShaderParameter ppp2 ( "u_ViewProjection", _engine->GetCamera()->GuiProjection(), UI::ShaderParameter::Type::Mat4 );
			UI::ShaderParameter ppp3 ( "u_Pixelborder", glm::vec2(0.05f, 0.3f), UI::ShaderParameter::Type::Vec2 );
			UI::ShaderParameter ppp4 ( "u_Textureborder", 0.3125f, UI::ShaderParameter::Type::Float );
			UI::ShaderParameter ppp5 ( "u_DebugMode", 0, UI::ShaderParameter::Type::Int );
			UI::ShaderParameter ppp6 ( "u_TextureCoords", glm::vec4(_guiTexture->_x, _guiTexture->_y,
					_guiTexture->_width, _guiTexture->_height), UI::ShaderParameter::Type::Vec4 );
			
			_mat3 = UI::Material { "gui-fancy" };
			_mat3.AddParameter(ppp1);
			_mat3.AddParameter(ppp2);
			_mat3.AddParameter(ppp3);
			_mat3.AddParameter(ppp4);
			_mat3.AddParameter(ppp5);
			_mat3.AddParameter(ppp6);
			
			_element3 = UI::Element { glm::vec2(-50.0f, -100.0f), 600, 100, 0.0f, UI::Pivot::TopRight,
				UI::Anchor::TopRight, nullptr, _guiTexture, _mat3 };
			_uiManager.CreateElement("el3", _element3);
			
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
			//cameraTransform.Rotate(-_rotSpeed * -mouseY, -_rotSpeed * mouseX, 0.0f);
			
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
			
			// Temporary gui stuff
			//_uiManager.GetElement("el1")->ApplyTranslation(glm::vec2(1.0f, 1.0f));
			_uiManager.GetElement("el1")->ApplyRotation(deltaTime*10.0f);
			//_uiManager.GetElement("el3")->ApplyRotation(deltaTime*10.0f);
			_uiManager.Update();
			
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
			_chunkRenderer.Render();
			
			_shaderManager.UseShader("gui-fancy");
			_guiAtlas.Bind();
			_uiManager.Render();

			_engine->GetWindow()->Update(deltaTime);
		}

	}

}