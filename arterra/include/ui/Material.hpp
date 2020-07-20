#pragma once

#include "PCH.hpp"
#include <glm/glm.hpp>

namespace arterra {

	namespace UI {

		/**
		 * @brief Data structure which contains the information for
		 * one shader parameter responsible for one shader `uniform`.
		 */
		struct ShaderParameter {
			union Value {
				int i;
				float_t f;
				glm::vec2 v2;
				glm::vec3 v3;
				glm::vec4 v4;
				glm::mat4 m4;

				Value(glm::vec2 _v2)
					: v2(_v2) {};
				Value(glm::vec3 _v3)
					: v3(_v3) {};
				Value(glm::vec4 _v4)
					: v4(_v4) {};
				Value(glm::mat4 _m4)
					: m4(_m4) {};
				Value(float_t _f)
					: f(_f) {};
				Value(int _i)
					: i(_i) {};
			};

			/// @brief The accepted types of `uniform` parameters.
			enum class Type { Int, Float, Vec2, Vec3, Vec4, Mat4 };

			/**
			 * @brief This ctor is called by `UI\::Material` when a new
			 * shader parameter is added.
			 * @param name The unique name of the uniform.
			 * @param value The initial value for the uniform.
			 * @param type The object type of the uniform.
			 */
			ShaderParameter(std::string name, Value value, Type type);

			/// @brief The name of the shader program `uniform`.
			std::string _name;
			/// @brief The value stored for this parameter.
			Value _value;
			/// @brief The type of parameter this `uniform` stores/uses.
			Type _type;
		};

		/**
		 * @brief Material data structure which holds information
		 * about a shader/texture combination. Provides the `AddParameter()`,
		 * `UpdateParameter()`, and `GetParameters()` methods to modify the
		 * shader parameters responsible for shader `uniforms`.
		 */
		class Material {

		public:
			/// ctor
			Material();
			/**
			 * @brief This ctor is called by `???` when ???.
			 * @param shaderIdentifier The unique identifier for a shader
			 * that this material will use.
			 */
			Material(std::string shaderIdentifier);

			/**
			 * @brief Adds a new shader parameter for a defined `uniform`.
			 * @param shaderParameter The data required for a specific uniform.
			 */
			void AddParameter(ShaderParameter shaderParameter);

			void UpdateParameter(std::string name, int value);
			void UpdateParameter(std::string name, float_t value);
			void UpdateParameter(std::string name, glm::vec2 value);
			void UpdateParameter(std::string name, glm::vec3 value);
			void UpdateParameter(std::string name, glm::vec4 value);
			void UpdateParameter(std::string name, glm::mat4 value);

			/**
			 * @brief Gets the identifier for this material's shader `program`.
			 * @return Value of the shaderIdentifier.
			 */
			std::string GetShaderIdentifier() { return _shaderIdentifier; }
			/**
			 * @brief Gets the data for a shader parameter.
			 * @param name The name of the shader parameter.
			 * @return `Pointer` to the specified `ShaderParameter` stored in this class.
			 */
			ShaderParameter* GetParameter(std::string name);
			/**
			 * @brief Gets all data for all shader parameters. This method is called
			 * by `UI\::Manager::Render()`.
			 * @return `Reference` to the member `_shaderParameters` of this class.
			 */
			inline std::vector<ShaderParameter>& GetParameters() { return _shaderParameters; }

		private:
			/// @brief The unique identifier for the shader to use.
			std::string _shaderIdentifier;
			/// @brief The list of all parameters for the shader, one
			/// for each `uniform` required.
			std::vector<ShaderParameter> _shaderParameters;
		};

	}

}