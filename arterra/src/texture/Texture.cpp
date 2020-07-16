#include "texture/Texture.hpp"

#include <SFML/Graphics/Image.hpp>

namespace arterra {

	TextureHandle::TextureHandle(std::string filepath) { Load(filepath); }

	TextureHandle::~TextureHandle() { glDeleteTextures(1, &_handle); }

	bool TextureHandle::Load(std::string filepath)
	{
		// Get a new texture handle
		glGenTextures(1, &_handle);
		// Load the texture using the resource manager
		auto result = ResourceManager::Get().Load(filepath);
		if (!result) {
			Logger::Get().Log(Logger::Debug, "Failed to load texture '", filepath, "'");
			return false;
		}
		sf::Image image;
		{
			// Get a handle to the resource, use stb to parse it properly
			auto dataHandle = ResourceManager::Get().GetHandle(filepath);
			image.loadFromMemory(dataHandle._resource->_data.data(), dataHandle._resource->_data.size());
			image.flipVertically();
			_width = image.getSize().x;
			_height = image.getSize().y;
		}
		// Bind the texture and buffer data
		glBindTexture(GL_TEXTURE_2D, _handle);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, static_cast<GLsizei>(_width), static_cast<GLsizei>(_height), 0,
			GL_RGBA, GL_UNSIGNED_BYTE, image.getPixelsPtr());
		// Auto-generate mipmaps
		glGenerateMipmap(GL_TEXTURE_2D);
		// Set scaling to nearest
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		// Success
		return true;
	}

}