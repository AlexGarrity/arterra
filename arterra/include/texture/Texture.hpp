#pragma once

#include "PCH.hpp"

#include <glad/glad.h>

namespace arterra {

	class TextureHandle {
	public:
		// Default ctor
		TextureHandle() = default;
		// RAII ctor
		TextureHandle(const std::string &filepath);
		~TextureHandle();

		bool Load(const std::string &filepath);

		GLint Handle() const;
		void Bind();

	private:
		GLuint _handle;
		int _width;
		int _height;
		int _channels;
	};

}