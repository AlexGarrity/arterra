#pragma once

#include "PCH.hpp"

#include <glad/glad.h>

#include <SFML/Window/Event.hpp>
#include <SFML/Window/Window.hpp>
#include <SFML/Window/Mouse.hpp>

namespace arterra {

	class Window {
	public:
		// ctor
		Window(int width, int height, std::string title);
		// dtor
		~Window();

		// Poll for events
		void PollEvents();
		// Swap buffers (advance frame)
		void SwapBuffers();
		// Set the clear colour using 0-1 floats
		void SetClearColour(float_t red, float_t green, float_t blue, float_t alpha);
		// Set the clear colour using 0-255 uints
		void SetClearColour(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha);
		// Clear the window
		void Clear();
		// Turn vsync on / off
		void SetVsync(bool vsync);
		// Lock/unlock the cursor to the window
		void SetLockCursor(bool lock);
		// Set the window to close
		void SetShouldClose(bool shouldClose);
		// Helper for updating
		void Update(float_t deltaTime);

		// Inline const getters
		inline uint32_t GetWidth() const { return _width; }
		inline uint32_t GetHeight() const { return _height; }
		inline bool IsVsyncEnabled() const { return _vsyncEnabled; }
		inline std::string GetTitle() const { return _title; }

		inline bool ShouldClose() const { return _shouldClose; }
		inline sf::Window& GetHandle() { return _window; }
		inline sf::Event& GetEvent() { return _event; }

	private:
		sf::Window _window;
		sf::Event _event;

		// Cache width, height, and title
		int _width;
		int _height;
		std::string _title;
		
		// Window options.
		bool _vsyncEnabled { true };
		bool _shouldClose { false };
		bool _cursorLocked { false };
	};

}