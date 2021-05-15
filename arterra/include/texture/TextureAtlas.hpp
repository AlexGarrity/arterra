#pragma once

#include "PCH.hpp"

#include <glad/glad.h>

namespace arterra {

	struct AtlasTexture;
	
	class TextureAtlas {
	public:
		TextureAtlas(const size_t width, const size_t height);
		~TextureAtlas();

		void Bind();

		bool LoadTexture(std::string filepath, std::string identifier);
		void AddTexture(size_t width, size_t height, std::vector<uint8_t>& data, std::string identifier);
		void AddTexture(size_t width, size_t height, const void* data, std::string identifier);

		AtlasTexture* GetTexture(std::string identifier);

	private:
		GLuint _glID;
		std::unordered_map<std::string, AtlasTexture> _textures;
		size_t _lastX = 0;
		size_t _lastY = 0;
		size_t _lastWidth = 0;
		size_t _lastHeight = 0;
		size_t _width;
		size_t _height;
	};

}