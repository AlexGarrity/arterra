#include "Engine.hpp"

#include "gamestate/Base.hpp"

namespace arterra {

	Engine::Engine()
	{
		_shouldExit = false;

		// Initialise engine components
		Logger::Get().Log(Logger::Debug, "Constructing window");
		_window = new Window(1280, 720, "arterra");
		Logger::Get().Log(Logger::Debug, "Constructing camera");
		_camera = new Camera();
		Logger::Get().Log(Logger::Debug, "Constructing renderer");
		_renderer = new Renderer(_camera);

		Logger::Get().Log(Logger::Debug, "Hooking input system to window");
		// Input system hook
		glfwSetKeyCallback(_window->GetHandle(), Input::KeyCallback);
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
			auto currentState = GetState();
			if (!currentState) {
				Logger::Get().Log(Logger::Error, "Engine has no state, exiting");
				SetShouldExit(true);
			} else {
				currentState->Input(_time.GetDeltaTime());
				currentState->Update(_time.GetDeltaTime());
				currentState->Render(_time.GetDeltaTime());
			}
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