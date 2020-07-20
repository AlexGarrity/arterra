#include "ui/Material.hpp"

namespace arterra {

	namespace UI {

		ShaderParameter::ShaderParameter(std::string name, Value value, Type type)
			: _name(name)
			, _value(value)
			, _type(type)
		{
		}

		Material::Material()
			: _shaderIdentifier("")
			, _shaderParameters {}
		{
		}

		Material::Material(std::string shaderIdentifier)
			: _shaderIdentifier(shaderIdentifier)
			, _shaderParameters {}
		{
		}

		void Material::AddParameter(ShaderParameter shaderParameter)
		{
			_shaderParameters.push_back(shaderParameter);
		}

		ShaderParameter* Material::GetParameter(std::string name)
		{
			// Iterate through all parameters until the names match, then return pointer.
			for (auto& parameter : _shaderParameters) {
				if (parameter._name == name)
					return &parameter;
			}
			Logger::Get().Log(Logger::Error, "[UI] Trying to get parameter `", name, "`, of shader `",
				_shaderIdentifier, "`, which does not exist!");
			return nullptr;
		}

		void Material::UpdateParameter(std::string name, int value)
		{
			// Iterate through all parameters until the names match.
			for (auto& parameter : _shaderParameters) {
				if (parameter._name == name) {
					parameter._value = value;
				}
			}
		}

		void Material::UpdateParameter(std::string name, float_t value)
		{
			// Iterate through all parameters until the names match.
			for (auto& parameter : _shaderParameters) {
				if (parameter._name == name) {
					parameter._value = value;
				}
			}
		}

		void Material::UpdateParameter(std::string name, glm::vec2 value)
		{
			// Iterate through all parameters until the names match.
			for (auto& parameter : _shaderParameters) {
				if (parameter._name == name) {
					parameter._value = value;
				}
			}
		}

		void Material::UpdateParameter(std::string name, glm::vec3 value)
		{
			// Iterate through all parameters until the names match.
			for (auto& parameter : _shaderParameters) {
				if (parameter._name == name) {
					parameter._value = value;
				}
			}
		}

		void Material::UpdateParameter(std::string name, glm::vec4 value)
		{
			// Iterate through all parameters until the names match.
			for (auto& parameter : _shaderParameters) {
				if (parameter._name == name) {
					parameter._value = value;
				}
			}
		}

		void Material::UpdateParameter(std::string name, glm::mat4 value)
		{
			// Iterate through all parameters until the names match.
			for (auto& parameter : _shaderParameters) {
				if (parameter._name == name)
					parameter._value = value;
			}
		}

	}

}