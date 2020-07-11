#pragma once

#include "PCH.hpp"

#include <glad/glad.h>

#include "util/Resource.hpp"

namespace arterra {

	struct AtlasTexture {
		float _width, _height;
		float _x, _y;

		AtlasTexture(float w, float h, float x, float y)
			: _width { w }
			, _height { h }
			, _x { x }
			, _y { y }
		{
		}

		AtlasTexture()
			: AtlasTexture(0.0f, 0.0f, 0.0f, 0.0f)
		{
		}

		AtlasTexture(size_t w, size_t h, size_t x, size_t y, size_t aW, size_t aH)
		{
			float fAw = static_cast<float>(aW);
			float fAh = static_cast<float>(aH);
			float fW = static_cast<float>(w);
			float fH = static_cast<float>(h);
			float fX = static_cast<float>(x);
			float fY = static_cast<float>(y);

			_width = fW / fAw;
			_height = fH / fAh;
			_x = fX / fAw;
			_y = fY / fAh;
		}
	};

	class TextureAtlas {
	public:
		TextureAtlas(const size_t width, const size_t height);
		~TextureAtlas();

		void Bind();

		bool LoadTexture(std::string filepath, std::string identifier);
		void AddTexture(size_t width, size_t height, std::vector<uint8_t>& data, std::string identifier);
		void AddTexture(size_t width, size_t height, void* data, std::string identifier);

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