#include "renderer/ShaderManager.hpp"

namespace arterra {

	ShaderManager::ShaderManager() {}

	bool ShaderManager::LoadShader(std::string fragPath, std::string vertPath, std::string identifier)
	{
		// Create a new shader program in the map.
		_shaders[identifier] = ShaderProgram();
		// Create the actual shaders.
		bool result = _shaders[identifier].Create(vertPath, fragPath);
		// Check if the shader was successfully compiled.
		if (result) {
			Logger::Get().Log(Logger::Debug, "Shader [", identifier, "] loaded successfully");
			return true;
		} else {
			Logger::Get().Log(Logger::Warning, "Shader [", identifier, "] failed to load");
			// It didn't work so don't save it.
			_shaders.erase(identifier);
			return false;
		}
	}

	bool ShaderManager::UnloadShader(std::string identifier)
	{
		// Find the requested shader by name.
		auto i = _shaders.find(identifier);
		// Make sure that the shader exists.
		if (i == _shaders.end()) {
			std::cerr << "Tried to unload shader '" << identifier << "' but it doesn't exist" << std::endl;
			return false;
		}

		// Delete the shader, this will also destroy the program/shaders in the destructors.
		_shaders.erase(i);
		std::cout << "Unloaded shader '" << identifier << "' successfully" << std::endl;
		return true;
	}

	bool ShaderManager::UseShader(std::string identifier)
	{
		// Set the active shader to the one specified by name.
		_activeShader = _shaders.find(identifier);
		// Make sure the shader exists.
		if (_activeShader == _shaders.end()) {
			std::cerr << "Tried to active shader '" << identifier << "' but it doesn't exist" << std::endl;
			return false;
		}
		// Set the active gl program.
		glUseProgram(GetShader(_activeShader));
		return true;
	}

	GLuint ShaderManager::GetShader(std::string identifier)
	{
		// Find the shader requested by name.
		auto i = _shaders.find(identifier);
		// Make sure the shader exists.
		if (i == _shaders.end()) {
			std::cerr << "Shader '" << identifier << "' does not exist" << std::endl;
			return 0;
		}
		// Return the shader program gl id.
		return i->second.GetProgram();
	}

	GLuint ShaderManager::GetShader(std::unordered_map<std::string, ShaderProgram>::iterator it)
	{
		// Return the shader program gl id.
		return it->second.GetProgram();
	}

}