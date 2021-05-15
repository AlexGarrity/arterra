#include "renderer/ShaderManager.hpp"

namespace arterra {

	ShaderManager::ShaderManager() {}

	bool ShaderManager::LoadShader(
		const std::string &fragPath, const std::string &vertPath, const std::string &identifier)
	{
		// Create a new shader program in the map.
		bool result = _shaders[identifier].Create(vertPath, fragPath);
		// Check if the shader was successfully compiled.
		if (result) {
			Logger::Debug( "Shader [", identifier, "] loaded successfully");
			return true;
		} else {
			Logger::Warning("Shader [", identifier, "] failed to load");
			// It didn't work so don't save it.
			_shaders.erase(identifier);
			return false;
		}
	}

	bool ShaderManager::UnloadShader(const std::string &identifier)
	{
		// Find the requested shader by name.
		auto i = _shaders.find(identifier);
		// Make sure that the shader exists.
		if (i == _shaders.end()) {
			return false;
		}

		// Delete the shader, this will also destroy the program/shaders in the destructors.
		_shaders.erase(i);
		return true;
	}

	bool ShaderManager::UseShader(const std::string& identifier)
	{
		// Set the active shader to the one specified by name.
		_activeShader = _shaders.find(identifier);
		// Make sure the shader exists.
		if (_activeShader == _shaders.end()) {
			return false;
		}
		// Set the active gl program.
		glUseProgram(GetShader(_activeShader));
		return true;
	}

	GLuint ShaderManager::GetShader(const std::string &identifier)
	{
		// Find the shader requested by name.
		auto i = _shaders.find(identifier);
		// Make sure the shader exists.
		if (i == _shaders.end()) {
			return 0;
		}
		// Return the shader program gl id.
		return i->second.GetProgram();
	}

	GLuint ShaderManager::GetShader(const std::unordered_map<std::string, ShaderProgram>::iterator it)
	{
		// Return the shader program gl id.
		return it->second.GetProgram();
	}

	ShaderProgram ShaderManager::ActiveProgram() const { return _activeShader->second; }

}
