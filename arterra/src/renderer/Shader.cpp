#include "renderer/Shader.hpp"

#include "util/Resource.hpp"

namespace arterra {

	Shader::Shader(ShaderType shaderType, const std::string &path)
		: _shaderType{ shaderType }
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

	Shader::~Shader()
	{
		glDeleteShader(_glID);
	}

	// Override for DataObject function
	void Shader::DumpToLog(const std::string &title) const
	{
		Logger::Log("\t", title, " - type: ", _shaderType, "; handle:", _glID);
	}

	GLuint Shader::Handle() const { return _glID; }

	ShaderProgram::ShaderProgram(const std::string &vertPath, const std::string &fragPath) { Create(vertPath, fragPath); }

	ShaderProgram::~ShaderProgram()
	{
		/* glDeleteProgram(_glID); */
	}

	bool ShaderProgram::Create(const std::string &vertPath, const std::string &fragPath)
	{
		// Create a new vertex shader.
		Shader vert{ Shader::Vertex, vertPath };
		// Make sure it has compiled correctly.
		if (!CheckShaderCompilation(vert.Handle()))
			return false;

		// Create a new fragment shader
		Shader frag{ Shader::Fragment, fragPath };
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

	GLuint ShaderProgram::GetProgram() const { return _glID; }

	GLint ShaderProgram::GetUniform(const char* name) { return glGetUniformLocation(_glID, name); }

	void ShaderProgram::SetUniform(std::string uniformName, glm::mat4 matrix)
	{
		auto uniform = GetUniform(uniformName.c_str());
		glUniformMatrix4fv(uniform, 1, GL_FALSE, glm::value_ptr(matrix));
	}

	void ShaderProgram::SetUniform(std::string uniformName, float value)
	{
		auto uniform = GetUniform(uniformName.c_str());
		glUniform1f(uniform, value);
	}

	void ShaderProgram::SetUniform(std::string uniformName, double value)
	{
		auto uniform = GetUniform(uniformName.c_str());
		glUniform1d(uniform, value);
	}

	void ShaderProgram::SetUniform(std::string uniformName, glm::vec2 vector)
	{
		auto uniform = GetUniform(uniformName.c_str());
		glUniform2fv(uniform, 1, glm::value_ptr(vector));
	}

	void ShaderProgram::SetUniform(std::string uniformName, glm::vec3 vector)
	{
		auto uniform = GetUniform(uniformName.c_str());
		glUniform3fv(uniform, 1, glm::value_ptr(vector));
	}

	void ShaderProgram::SetUniform(std::string uniformName, glm::vec4 vector)
	{
		auto uniform = GetUniform(uniformName.c_str());
		glUniform4fv(uniform, 1, glm::value_ptr(vector));
	}

	void ShaderProgram::SetUniform(std::string uniformName, GLint value)
	{
		auto uniform = GetUniform(uniformName.c_str());
		glUniform1i(uniform, value);
	}

	void ShaderProgram::DumpToLog(const std::string &title) const { Logger::Log("\t", title, " - ", "handle: ", _glID); }

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
			Logger::Warning("Shader failed to compile");
			Logger::Warning("Shader error log: ", buffer);
			return false;
		}
		Logger::Debug( "Shader compiled successfully");
		return true;
	}

}
