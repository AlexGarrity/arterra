#pragma once

#include "PCH.hpp"

#include "util/Time.hpp"

namespace arterra {
	class ThreadManager;
	class Window;
	class Input;
	class Renderer;
	class Camera;

	// Forward decl of Base gamestate to prevent circular inclusion
	namespace gamestate {
		class Base;
	}

	class Engine {

	public:
		Engine();
		~Engine();

		void Run();

		gamestate::Base* GetState() const;
		void PushState(gamestate::Base* state);
		void PopState();

		const bool StateQueueEmpty() const;
		void SetShouldExit(bool b);

		Window* GetWindow() const;
		Renderer* GetRenderer() const;
		Camera* GetCamera() const;
		Input* GetInput() const;
		ThreadManager* GetThreadManager() const;

	private:
		bool _shouldExit = false;
		std::queue<gamestate::Base*> _stateQueue;

		// TODO - Convert to using C++11 ptrs
		Window* _window;
		Renderer* _renderer;
		Camera* _camera;
		Input* _input;
		ThreadManager* _threadManager;

		Time _time;
	};

}