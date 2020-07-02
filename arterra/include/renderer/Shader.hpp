#pragma once
#include "PCH.hpp"

#include "util/Resource.hpp"

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace arterra {

	class ShaderProgram : public DataObject {

		struct Shader : public DataObject {

			// The type of shader.
			enum ShaderType { Fragment, Vertex, Geometry };
			ShaderType _shaderType;
			// The unique opengl id for this individual shader.
			GLuint _glID;

			// ctor runs the gl functions to create a new shader.
			Shader(ShaderType shaderType, std::string path)
				: _shaderType { shaderType }
			{
				// Load the shader file resource.
				bool loadResult = ResourceManager::Get().Load(path);
				if (!loadResult)
					return;

				// Get a handle to the file resource.
				auto dataHandle = ResourceManager::Get().GetHandle(path);
				// Get the data from the resource.
				auto src = std::string(dataHandle._resource->_data.begin(), dataHandle._resource->_data.end());
				// Convert uint8_t* to char*
				const char* cSrc = src.c_str();

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

				// Feed the data in and compile the shader.
				glShaderSource(_glID, 1, &cSrc, nullptr);
				glCompileShader(_glID);
			}

			// dtor calls the glDeleteShader function.
			~Shader() { glDeleteShader(_glID); }

			void DumpToLog(std::string title = "Shader") override
			{
				Logger::Get().Log("\t", title, " - type: ", _shaderType, "; handle:", _glID);
			}

			std::vector<uint8_t> Serialize() override { return {}; }
		};

	public:
		ShaderProgram() = default;

		// Create a new shader program containing a vertex and fragment shader.
		ShaderProgram(std::string vertShaderPath, std::string fragShaderPath)
		{
			Create(vertShaderPath, fragShaderPath);
		};

		// dtor calls the glDeleteProgram function.
		~ShaderProgram() { glDeleteProgram(_glID); }

		// Create a set of new shaders.
		// bool returns compilation status.
		bool Create(std::string vertPath, std::string fragPath);

		// Get the gl id for this program.
		inline GLuint GetProgram() const { return _glID; }

		// Get the gl reference to the specified uniform by name.
		inline GLint GetUniform(const char* name) { return glGetUniformLocation(_glID, name); }

		// Set a shader uniform for the specified name.
		inline void SetUniform(std::string uniformName, glm::mat4 matrix)
		{
			GLint uniform = GetUniform(uniformName.c_str());
			glUniformMatrix4fv(uniform, 1, GL_FALSE, glm::value_ptr(matrix));
		}

		// Set a shader uniform for the specified name.
		inline void SetUniform(std::string uniformName, float_t value)
		{
			GLint uniform = GetUniform(uniformName.c_str());
			glUniform1f(uniform, value);
		}

		// Set a shader uniform for the specified name.
		inline void SetUniform(std::string uniformName, double_t value)
		{
			GLint uniform = GetUniform(uniformName.c_str());
			glUniform1d(uniform, value);
		}

		// Set a shader uniform for the specified name.
		inline void SetUniform(std::string uniformName, glm::vec3 vector)
		{
			GLint uniform = GetUniform(uniformName.c_str());
			glUniform3fv(uniform, 1, glm::value_ptr(vector));
		}

		void DumpToLog(std::string title = "ShaderProgram")
		{
			Logger::Get().Log("\t", title, " - ", "handle: ", _glID);
		}

	private:
		// Checks if the shader was compiled successfully.
		static bool CheckShaderCompilation(GLuint shader);
		// The unique opengl id for this buffer.
		GLuint _glID;
	};

};