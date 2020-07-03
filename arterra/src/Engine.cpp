#include "Engine.hpp"

#include "gamestate/Base.hpp"

namespace arterra {

    Engine::Engine() {
        _shouldExit = false;
    }

    void Engine::Run() {
        while (!_shouldExit) {
            auto currentState = GetState();
            if (!currentState) {
                Logger::Get().Log(Logger::Error, "Engine has no state, exiting");
            }
            currentState->Input();
            currentState->Update();
            currentState->Render();
        }
    }

    const std::shared_ptr<gamestate::Base> Engine::GetState() const {
        if (StateQueueEmpty()) {
            return nullptr;
        }
        return _stateQueue.front();
    }

    void Engine::PushState(std::shared_ptr<gamestate::Base> state) {
        _stateQueue.push(state);
    }

    void Engine::PopState() {
        if (!StateQueueEmpty()) {
            _stateQueue.pop();
        }
    }


}