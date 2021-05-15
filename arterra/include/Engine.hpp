#pragma once

#include "PCH.hpp"

#include "util/Time.hpp"

#include "renderer/Camera.hpp"
#include "renderer/Renderer.hpp"

#include "window/Input.hpp"
#include "window/Window.hpp"

namespace arterra {

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

	private:
		bool _shouldExit = false;
		std::queue<gamestate::Base*> _stateQueue;

		// TODO - Convert to using C++11 ptrs
		Window* _window;
		Renderer* _renderer;
		Camera* _camera;
		Input* _input;

		Time _time;
	};

}