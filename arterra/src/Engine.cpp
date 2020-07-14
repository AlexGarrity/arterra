#include "Engine.hpp"

#include "gamestate/Base.hpp"

namespace arterra {

	Engine::Engine()
	{
		_shouldExit = false;

		// Initialise engine components
		Logger::Get().Log(Logger::Debug, "Constructing window");
		_window = new Window(1280, 720, "arterra");
		Logger::Get().Log(Logger::Debug, "Constructing input system");
		_input = new Input(_window->GetEvent(), _window);
		Logger::Get().Log(Logger::Debug, "Constructing camera");
		_camera = new Camera();
		Logger::Get().Log(Logger::Debug, "Constructing renderer");
		_renderer = new Renderer();
	}

	Engine::~Engine()
	{
		Logger::Get().Log(Logger::Debug, "Destroying window");
		delete _renderer;
		delete _camera;
		delete _window;
	}

	void Engine::Run()
	{
		Logger::Get().Log(Logger::Debug, "Beginning main engine loop");
		while (!_shouldExit) {
			_camera->Update(*_window, _time.GetDeltaTime());
			_input->Update(_time.GetDeltaTime());

			auto currentState = GetState();
			if (!currentState) {
				Logger::Get().Log(Logger::Error, "Engine has no state, exiting");
				SetShouldExit(true);
			} else {
				currentState->Input(_time.GetDeltaTime());
				currentState->Update(_time.GetDeltaTime());
				currentState->Render(_time.GetDeltaTime());
			}
			// Must be called after the state update, since this sets mouse data
			// for use in the next iteration.
			
			_time.CalculateDeltaTime();
		}
	}

	gamestate::Base* Engine::GetState() const
	{
		if (StateQueueEmpty()) {
			return nullptr;
		}
		return _stateQueue.front();
	}

	void Engine::PushState(gamestate::Base* state) { _stateQueue.push(state); }

	void Engine::PopState()
	{
		if (!StateQueueEmpty()) {
			_stateQueue.pop();
		}
	}

}