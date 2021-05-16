#include "Engine.hpp"

#include "gamestate/Base.hpp"

#include "renderer/Camera.hpp"
#include "renderer/Renderer.hpp"

#include "window/Input.hpp"
#include "window/Window.hpp"

#include "thread/ThreadManager.hpp"

namespace arterra {

	Engine::Engine()
	{
		_shouldExit = false;

		// Initialise engine components
		Logger::Debug( "Constructing window");
		_window = new Window(1280, 720, "arterra");
		Logger::Debug( "Constructing input system");
		_input = new Input(_window->GetEvent(), _window);
		Logger::Debug( "Constructing camera");
		_camera = new Camera();
		Logger::Debug( "Constructing renderer");
		_renderer = new Renderer();
		Logger::Debug("Constructing thread manager");
		_threadManager = new ThreadManager();
		_threadManager->CreateThreads(7);
	}

	Engine::~Engine()
	{
		Logger::Debug( "Destroying window");
		delete _renderer;
		delete _camera;
		delete _window;
	}

	void Engine::Run()
	{
		Logger::Debug( "Beginning main engine loop");
		while (!_shouldExit) {
			//_camera->Update(*_window, _time.GetDeltaTime());
			_input->Update(_time.GetDeltaTime());

			auto currentState = GetState();
			if (!currentState) {
				Logger::Error("Engine has no state, exiting");
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

	const bool Engine::StateQueueEmpty() const { return _stateQueue.empty(); }
	
	void Engine::SetShouldExit(bool b) { _shouldExit = b; }
	
	Window* Engine::GetWindow() const { return _window; }
	
	Renderer* Engine::GetRenderer() const { return _renderer; }
	
	Camera* Engine::GetCamera() const { return _camera; }
	
	Input* Engine::GetInput() const { return _input; }
	
	ThreadManager* Engine::GetThreadManager() const { return _threadManager; }

}
