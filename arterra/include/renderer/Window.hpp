#pragma once

#include "PCH.hpp"
#include <glfw/glfw3.h>

namespace arterra {

    class Window {
        public:
            Window(uint32_t width, uint32_t height, std::string title);

        private:
            // Unique ptr to window
            // Automatically deleted when the class is destroyed.  Can only be one instance.
            std::unique_ptr<GLFWwindow> _window;

            uint32_t _width;
            uint32_t _height;
            std::string _title;
    };

}