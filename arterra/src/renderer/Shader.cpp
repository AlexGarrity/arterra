#include "renderer/Shader.hpp"

namespace arterra {

	Shader::Shader(ShaderType shaderType, std::string path)
		: _shaderType { shaderType }
	{
		// Load the shader resource
		auto loadResult = ResourceManager::Get().Load(path);
		// If it failed to load, return
		if (!loadResult)
			return;

		// Get a handle to the resource
		auto dataHandle = ResourceManager::Get().GetHandle(path);
		// Get the data from the resource
		auto src = std::string(dataHandle._resource->_data.begin(), dataHandle._resource->_data.end());
		// Convert uint8_t* to char*
		const char* cSrc = src.c_str();

		// Generate the right kind of shader based on the type
		switch (shaderType) {
			case ShaderType::Fragment:
				_glID = glCreateShader(GL_FRAGMENT_SHADER);
				break;
			case ShaderType::Vertex:
				_glID = glCreateShader(GL_VERTEX_SHADER);
				break;
			case ShaderType::Geometry:
				_glID = glCreateShader(GL_GEOMETRY_SHADER);
				break;
		}

		// Feed in the source code
		glShaderSource(_glID, 1, &cSrc, nullptr);
		// Compile the shader
		glCompileShader(_glID);
	}

	// Override for DataObject function
	void Shader::DumpToLog(std::string title)
	{
		Logger::Get().Log("\t", title, " - type: ", _shaderType, "; handle:", _glID);
	}

	ShaderProgram::ShaderProgram(std::string vertPath, std::string fragPath) { Create(vertPath, fragPath); }

	ShaderProgram::~ShaderProgram() { /* glDeleteProgram(_glID); */ }

	bool ShaderProgram::Create(std::string vertPath, std::string fragPath)
	{

		// Create a new vertex shader.
		Shader vert { Shader::Vertex, vertPath };
		// Make sure it has compiled correctly.
		if (!CheckShaderCompilation(vert.Handle()))
			return false;

		// Create a new fragment shader
		Shader frag { Shader::Fragment, fragPath };
		// Make sure it has compiled correctly.
		if (!CheckShaderCompilation(frag.Handle()))
			return false;

		// Create a new shader program.
		_glID = glCreateProgram();
		// Attach the two shaders to the program.
		glAttachShader(_glID, vert.Handle());
		glAttachShader(_glID, frag.Handle());
		// Bind the fragment data.
		glBindFragDataLocation(_glID, 0, "outColor");
		// Link the program.
		glLinkProgram(_glID);
		
		return true;
	}

	bool ShaderProgram::CheckShaderCompilation(GLuint shader)
	{
		GLint result;
		char buffer[512];
		// Get the compile status.
		glGetShaderiv(shader, GL_COMPILE_STATUS, &result);
		// Get the compile log.
		glGetShaderInfoLog(shader, 512, nullptr, buffer);

		// If the compilation was unsuccessful, log it.
		if (result == GL_FALSE) {
			Logger::Get().Log(Logger::Warning, "Shader failed to compile");
			Logger::Get().Log(Logger::Warning, "Shader error log: ", buffer);
			return false;
		}
		Logger::Get().Log(Logger::Debug, "Shader compiled successfully");
		return true;
	}

}