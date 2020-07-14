#pragma once
#include "PCH.hpp"

#include "util/Resource.hpp"
#include "util/DataObject.hpp"

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace arterra {

	class Shader : public DataObject {
	public:
		enum ShaderType { Fragment, Vertex, Geometry };

		Shader(ShaderType shaderType, std::string path);
		~Shader() { glDeleteShader(_glID); }
		virtual void DumpToLog(std::string title = "Shader") override;
		inline GLuint Handle() { return _glID; }

	private:
		ShaderType _shaderType;
		GLuint _glID;
	};

	class ShaderProgram : public DataObject {

	public:
		ShaderProgram() = default;

		// Create a new shader (RAII)
		ShaderProgram(std::string vertShaderPath, std::string fragShaderPath);

		// Delete the program
		~ShaderProgram();

		// Create a new shader - return whether or not it worked
		bool Create(std::string vertPath, std::string fragPath);

		// Inline returns the program handle
		inline GLuint GetProgram() const { return _glID; }

		// Inlines for setting uniforms in shaders
		inline GLint GetUniform(const char* name) { return glGetUniformLocation(_glID, name); }

		inline void SetUniform(std::string uniformName, glm::mat4 matrix)
		{
			auto uniform = GetUniform(uniformName.c_str());
			glUniformMatrix4fv(uniform, 1, GL_FALSE, glm::value_ptr(matrix));
		}

		inline void SetUniform(std::string uniformName, float_t value)
		{
			auto uniform = GetUniform(uniformName.c_str());
			glUniform1f(uniform, value);
		}

		inline void SetUniform(std::string uniformName, double_t value)
		{
			auto uniform = GetUniform(uniformName.c_str());
			glUniform1d(uniform, value);
		}

		inline void SetUniform(std::string uniformName, glm::vec3 vector)
		{
			auto uniform = GetUniform(uniformName.c_str());
			glUniform3fv(uniform, 1, glm::value_ptr(vector));
		}

		inline void SetUniform(std::string uniformName, glm::vec4 vector)
		{
			auto uniform = GetUniform(uniformName.c_str());
			glUniform4fv(uniform, 1, glm::value_ptr(vector));
		}

		inline void SetUniform(std::string uniformName, GLint value)
		{
			auto uniform = GetUniform(uniformName.c_str());
			glUniform1i(uniform, value);
		}

		void DumpToLog(std::string title = "ShaderProgram")
		{
			Logger::Get().Log("\t", title, " - ", "handle: ", _glID);
		}

	private:
		static bool CheckShaderCompilation(GLuint shader);

		std::vector<Shader> _shaders;
		GLuint _glID;
	};

};