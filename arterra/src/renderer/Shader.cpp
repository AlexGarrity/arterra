#include "renderer/Shader.hpp"

namespace arterra {

	bool ShaderProgram::Create(std::string vertPath, std::string fragPath)
	{

		// Create a new vertex shader.
		Shader vert { Shader::Vertex, vertPath };
		// Make sure it has compiled correctly.
		if (!CheckShaderCompilation(vert._glID))
			return false;

		// Create a new fragment shader
		Shader frag { Shader::Fragment, fragPath };
		// Make sure it has compiled correctly.
		if (!CheckShaderCompilation(frag._glID))
			return false;

		// Create a new shader program.
		_glID = glCreateProgram();
		// Attach the two shaders to the program.
		glAttachShader(_glID, vert._glID);
		glAttachShader(_glID, frag._glID);
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