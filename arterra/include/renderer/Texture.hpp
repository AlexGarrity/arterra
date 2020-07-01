#pragma once

#include "PCH.hpp"

#include <glad/glad.h>

#include "util/Resource.hpp"

#include "stb_image.h"

namespace arterra{

	class TextureHandle {
		public:

			TextureHandle() = default;

			TextureHandle(std::string filepath) {
				Load(filepath);
			}

			~TextureHandle() {
				glDeleteTextures(1, &_handle);
			}

			bool Load(std::string filepath) {
				// Get a new texture handle
				glGenTextures(1, &_handle);
				// Load the texture using the resource manager
				auto result = Resource::Get().Load(filepath);
				if (!result) {
					Logger::Get().Log(Logger::Debug, "Failed to load texture '", filepath, "'");
					return false;
				}

				uint8_t *data;
				{
					auto dataHandle = Resource::Get().Get(filepath);
					data = stbi_load_from_memory(
						dataHandle._resource->_data.data(), dataHandle._resource->_data.size(), &_width, &_height, &_channels, 3
					);
				}
				// Bind the texture and buffer data
				glBindTexture(GL_TEXTURE_2D, _handle);
				glTexImage2D(
					GL_TEXTURE_2D, 0, GL_RGB, static_cast<GLsizei>(_width), 
					static_cast<GLsizei>(_height), 0, GL_RGB, GL_UNSIGNED_BYTE, data);
				glGenerateMipmap(GL_TEXTURE_2D);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

				stbi_image_free(data);
				return true;
			}

			inline GLuint Handle() const { return _handle; }

		private:
			GLuint _handle;
			int32_t _width;
			int32_t _height;
			int32_t _channels;
	};


}