#include "gamestate/Test.hpp"

namespace arterra {

    namespace gamestate {

        Test::Test(Engine *engine) : Base(engine) {
            _engine->GetWindow()->SetVsync(true);
            _engine->GetWindow()->SetClearColour(1.0f, 0.0f, 1.0f, 1.0f);
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
            _engine->GetRenderer()->Update();

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
            _engine->GetWindow()->Update(deltaTime);
        }

    }


}