#pragma once

#include "PCH.hpp"
#include <glfw/glfw3.h>

namespace arterra {

    struct GLFWwindowDeleter {
        void operator()(GLFWwindow *window) {
            glfwDestroyWindow(window);
        }
    };

    class Window {
        public:
            // ctor
            Window(uint32_t width, uint32_t height, std::string title);
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
            // Set the window to close
            void SetShouldClose(bool shouldClose);
            // Helper for updating
            void Update();

            // Inline const getters
            inline uint32_t GetWidth() const { return _width; }
            inline uint32_t GetHeight() const { return _height; }
            inline bool IsVsyncEnabled() const { return _vsyncEnabled; }
            inline std::string GetTitle() const { return _title; }

            inline bool ShouldClose() const { return _shouldClose; }
            inline bool IsKeyPressed(uint32_t glfwKeycode) const { return (glfwGetKey(_window.get(), glfwKeycode) == GLFW_PRESS); }
            inline GLFWwindow *GetHandle() { return _window.get(); }

        private:
            // Unique ptr to window
            // Automatically deleted when the class is destroyed.  Can only be one instance.
            std::unique_ptr<GLFWwindow, GLFWwindowDeleter> _window;

            // Cache width, height, and title
            uint32_t _width;
            uint32_t _height;
            std::string _title;
            bool _vsyncEnabled;
            bool _shouldClose;
    };

}