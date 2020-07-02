#pragma once

#include "PCH.hpp"
#include "renderer/Shader.hpp"

namespace arterra {

	class ShaderManager {
	public:
		ShaderManager();

		// Load a shader using the given paths and save it to the shader index.
		bool LoadShader(std::string fragPath, std::string vertPath, std::string identifier);
		// Unload a shader and delete it from memory.
		bool UnloadShader(std::string identifier);
		// Set a shader as active.
		bool UseShader(std::string identifier);

		// Get a shader program gl id by name.
		GLuint GetShader(std::string identifier);
		// Get a shader program gl id by index position in the index.
		GLuint GetShader(std::unordered_map<std::string, ShaderProgram>::iterator it);

		inline ShaderProgram ActiveProgram() const { return _activeShader->second; }

	private:
		// Map of loaded shader programs.
		std::unordered_map<std::string, ShaderProgram> _shaders;
		std::unordered_map<std::string, ShaderProgram>::iterator _activeShader;
	};

}