#include "renderer/Texture.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"


namespace arterra {

    TextureHandle::TextureHandle(std::string filepath) {
        Load(filepath);
    }

    TextureHandle::~TextureHandle() {
        glDeleteTextures(1, &_handle);
    }

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

        uint8_t* data;
        {
            // Get a handle to the resource, use stb to parse it properly
            auto dataHandle = ResourceManager::Get().GetHandle(filepath);
            data = stbi_load_from_memory(
                dataHandle._resource->_data.data(), static_cast<int>(dataHandle._resource->_data.size()), &_width, &_height, &_channels, 3);
        }
        // Bind the texture and buffer data
        glBindTexture(GL_TEXTURE_2D, _handle);
        glTexImage2D(
            GL_TEXTURE_2D, 0, GL_RGB, static_cast<GLsizei>(_width),
            static_cast<GLsizei>(_height), 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        // Auto-generate mipmaps
        glGenerateMipmap(GL_TEXTURE_2D);
        // Set scaling to nearest
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        // Free image data memory
        stbi_image_free(data);
        // Success
        return true;
    }


}