#pragma once
#include "PCH.hpp"

#include "util/DataObject.hpp"

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

namespace arterra {

	class Shader : public DataObject {
	public:
		enum ShaderType { Fragment, Vertex, Geometry };

		Shader(ShaderType shaderType, const std::string &path);
		~Shader();
		void DumpToLog(const std::string &title = "Shader") const override;
		GLuint Handle() const;

	private:
		const ShaderType _shaderType;
		GLuint _glID;
	};

	class ShaderProgram : public DataObject {

	public:
		ShaderProgram() = default;
		
		// Create a new shader (RAII)
		ShaderProgram(const std::string &vertShaderPath, const std::string &fragShaderPath);

		// Delete the program
		~ShaderProgram();

		// Create a new shader - return whether or not it worked
		bool Create(const std::string &vertPath, const std::string &fragPath);

		// Inline returns the program handle
		GLuint GetProgram() const;

		// Inlines for setting uniforms in shaders
		GLint GetUniform(const char* name);

		void SetUniform(std::string uniformName, glm::mat4 matrix);
		void SetUniform(std::string uniformName, float value);
		void SetUniform(std::string uniformName, double value);
		void SetUniform(std::string uniformName, glm::vec2 vector);
		void SetUniform(std::string uniformName, glm::vec3 vector);
		void SetUniform(std::string uniformName, glm::vec4 vector);
		void SetUniform(std::string uniformName, GLint value);

		void DumpToLog(const std::string &title = "ShaderProgram") const override;

	private:
		static bool CheckShaderCompilation(GLuint shader);

		std::vector<Shader> _shaders;
		GLuint _glID;
	};

};