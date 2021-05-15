#pragma once

#include "PCH.hpp"

#include <glad/glad.h>

namespace arterra {

	struct AtlasTexture;
	
	class TextureAtlas {
	public:
		TextureAtlas(size_t width, size_t height);
		~TextureAtlas();

		void Bind();

		bool LoadTexture(const std::string &filepath, const std::string &identifier);
		void AddTexture(size_t width, size_t height, std::vector<uint8_t>& data, const std::string &identifier);
		void AddTexture(size_t width, size_t height, const void* data, const std::string &identifier);

		AtlasTexture* GetTexture(const std::string &identifier);

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