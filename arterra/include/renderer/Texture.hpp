#pragma once

#include "PCH.hpp"

#include <glad/glad.h>

#include "util/Resource.hpp"

namespace arterra {

	class TextureHandle {
	public:
		// Default ctor
		TextureHandle() = default;
		// RAII ctor
		TextureHandle(std::string filepath);
		~TextureHandle();

		bool Load(std::string filepath);


		inline GLuint Handle() const { return _handle; }
		inline void Bind() { glBindTexture(GL_TEXTURE_2D, _handle); }

	private:
		GLuint _handle;
		int _width;
		int _height;
		int _channels;
	};

}