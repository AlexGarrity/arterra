#include "texture/TextureAtlas.hpp"

#include <SFML/Graphics/Image.hpp>
#include "util/Resource.hpp"
#include "texture/AtlasTexture.hpp"

namespace arterra {

	TextureAtlas::TextureAtlas(const size_t width, const size_t height)
	{
		_width = width;
		_height = height;
		// Generate texture ID
		glGenTextures(1, &_glID);
		// Fill an array with 0s
		std::vector<uint8_t> blank;
		blank.resize(width * height * 4);
		// Buffer the data to the image
		glBindTexture(GL_TEXTURE_2D, _glID);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, blank.data());
		// Set scaling to nearest
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		std::vector<uint8_t> errorTexture;
		errorTexture.resize(16 * 16 * 4);
		bool coloured = false;
		for (auto y = 0; y < 16; ++y) {
			coloured = !coloured;
			for (auto x = 0; x < 16; ++x) {
				coloured = !coloured;
				if (coloured) {
					errorTexture[4 * (x + 16 * y) + 0] = 255;
					errorTexture[4 * (x + 16 * y) + 1] = 0;
					errorTexture[4 * (x + 16 * y) + 2] = 255;
					errorTexture[4 * (x + 16 * y) + 3] = 255;
				} else {
					errorTexture[4 * (x + 16 * y) + 0] = 0;
					errorTexture[4 * (x + 16 * y) + 1] = 0;
					errorTexture[4 * (x + 16 * y) + 2] = 0;
					errorTexture[4 * (x + 16 * y) + 3] = 255;
				}
			}
		}
		AddTexture(16, 16, errorTexture, "error");
	}

	TextureAtlas::~TextureAtlas()
	{
		glDeleteTextures(1, &_glID);
	}

	void TextureAtlas::Bind()
	{
		glBindTexture(GL_TEXTURE_2D, _glID);
	}

	bool TextureAtlas::LoadTexture(std::string filepath, std::string identifier)
	{
		// Load the texture using the resource manager
		auto result = ResourceManager::Get().Load(filepath);
		if (!result) {
			Logger::Get().Log(Logger::Severity::Error, "Texture not found: <" + identifier + ">");
			return false;
		}
		sf::Image image;
		{
			// Get a handle to the resource, use SFML to parse it properly
			auto dataHandle = ResourceManager::Get().GetHandle(filepath);
			image.loadFromMemory(dataHandle._resource->_data.data(), dataHandle._resource->_data.size());
			image.flipVertically();
		}
		AddTexture(image.getSize().x, image.getSize().y, image.getPixelsPtr(), identifier);
		return true;
	}

	void TextureAtlas::AddTexture(size_t width, size_t height, std::vector<uint8_t>& data, std::string identifier)
	{
		if (_textures.find(identifier) != _textures.end())
			return;
		auto posX = _lastX + _lastWidth;
		auto posY = _lastY;
		if (posX + width > _width) {
			posX = 0;
			posY += _lastHeight;
		}
		if (posY + height > _height)
			return;
		glBindTexture(GL_TEXTURE_2D, _glID);
		glTexSubImage2D(GL_TEXTURE_2D, 0, posX, posY, width, height, GL_RGBA, GL_UNSIGNED_BYTE, data.data());
		_textures.emplace(identifier, AtlasTexture { width, height, posX, posY, _width, _height });
		_lastX = posY;
		_lastY = posX;
		_lastWidth = width;
		_lastHeight = height;
	}

	void TextureAtlas::AddTexture(size_t width, size_t height, const void* data, std::string identifier)
	{
		if (_textures.find(identifier) != _textures.end())
			return;
		auto posX = _lastX + _lastWidth;
		auto posY = _lastY;
		if (posX + width > _width) {
			posX = 0;
			posY += _lastHeight;
		}
		if (posY + height > _height)
			return;
		glBindTexture(GL_TEXTURE_2D, _glID);
		glTexSubImage2D(GL_TEXTURE_2D, 0, posX, posY, width, height, GL_RGBA, GL_UNSIGNED_BYTE, data);
		_textures.emplace(identifier, AtlasTexture { width, height, posX, posY, _width, _height });
		_lastX = posY;
		_lastY = posX;
		_lastWidth = width;
		_lastHeight = height;
	}

	AtlasTexture* TextureAtlas::GetTexture(std::string identifier)
	{
		auto it = _textures.find(identifier);
		if (it == _textures.end())
			return &_textures["error"];
		return &it->second;
	}

}
