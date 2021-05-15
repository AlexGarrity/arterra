#pragma once

#include "PCH.hpp"
#include "renderer/Shader.hpp"

namespace arterra {

	class ShaderManager {
	public:
		ShaderManager();

		// Load a shader using the given paths and save it to the shader index.
		bool LoadShader(const std::string &fragPath, const std::string &vertPath, const std::string &identifier);
		// Unload a shader and delete it from memory.
		bool UnloadShader(const std::string &identifier);
		// Set a shader as active.
		bool UseShader(const std::string &identifier);

		// Get a shader program gl id by name.
		GLuint GetShader(const std::string& identifier);
		// Get a shader program gl id by index position in the index.
		GLuint GetShader(std::unordered_map<std::string, ShaderProgram>::iterator it);

		ShaderProgram ActiveProgram() const;

	private:
		// Map of loaded shader programs.
		std::unordered_map<std::string, ShaderProgram> _shaders;
		std::unordered_map<std::string, ShaderProgram>::iterator _activeShader;
	};

}