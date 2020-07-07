#include "gamestate/Test.hpp"

namespace arterra {

    namespace gamestate {

        Test::Test(Engine *engine) : Base(engine), _subChunk(0, 0, 0, _cubeModel) {
            _engine->GetWindow()->SetVsync(true);
            _engine->GetWindow()->SetClearColour(1.0f, 0.0f, 1.0f, 1.0f);
			
			// Load the basic shader and use it
			_shaderManager.LoadShader("shaders/basic.frag", "shaders/basic.vert", "basic");
			_shaderManager.UseShader("basic");
			
			// Create cube model.
			_cubeModel.Create("models/cube.mobj");
			
			
			
			Direction direction = Direction::PosX;
			
			for(auto block: _subChunk.GetBlocks()) {
				if (!block)
					continue;
				
				auto& model = block->_model.GetFace(direction);
				
				for (auto i = 0; i < model.size(); i += 3) {
					model[i + 0] += static_cast<float>(block->GetPosition()._x);
					model[i + 1] += static_cast<float>(block->GetPosition()._y);
					model[i + 2] += static_cast<float>(block->GetPosition()._z);
				}
				
				_meshVertices.insert(_meshVertices.end(), model.begin(), model.end());
			}
			
			vao.Bind();
			
			vbo.Create(_meshVertices, 3, GL_FLOAT);
			vao.AddBuffer(vbo);
			
        }

        void Test::Input(float_t deltaTime) {
            Input::Update(deltaTime);

            // Close engine with [Esc]
            if (_engine->GetWindow()->IsKeyPressed(GLFW_KEY_ESCAPE)) {
                _engine->SetShouldExit(true);
            }
        }

        void Test::Update(float_t deltaTime) {
            _engine->GetCamera()->Update(*_engine->GetWindow(), deltaTime);
            
			
			// Set the camera view projection so the world renders in perspective
			_shaderManager.ActiveProgram().SetUniform("viewProjection", _engine->GetCamera()->ViewProjection());
			_shaderManager.ActiveProgram().SetUniform("fragmentColour", {0.2f, 1.0f, 1.0f, 1.0f});
			
            // Every 5 seconds, perform garbage collection
            _timeToResourceUnload -= deltaTime;
            if (_timeToResourceUnload < 0.0f) {
                ResourceManager::Get().Unload();
                _timeToResourceUnload = 5.0f;
            }
        }

        void Test::Render(float_t deltaTime) {
            // Clear the window
            _engine->GetWindow()->Clear();
            _engine->GetRenderer()->DrawTestCube();
			
			
			_shaderManager.UseShader("basic");
			_shaderManager.ActiveProgram().SetUniform("fragmentTexture", 0);
			/* 
			for(auto block: _subChunk.GetBlocks()) {
				if (!block)
					continue;
				// Draw each block
				glm::mat4 modelMat (1.0f);
				BlockPosition pos = block->GetPosition();
				modelMat = glm::translate(modelMat, glm::vec3(pos._x, pos._y, pos._z));
				modelMat = glm::scale(modelMat, glm::vec3(0.5f));
				_shaderManager.ActiveProgram().SetUniform("modelPos", modelMat);
				
				block->_model.Bind();
				glDrawArrays(GL_TRIANGLES, 0, 36);
			}
			 */
			
			vao.Bind();
			glDrawArrays(GL_TRIANGLES, 0, vao.GetVertexCount());
			
			
            _engine->GetWindow()->Update(deltaTime);
        }

    }


}