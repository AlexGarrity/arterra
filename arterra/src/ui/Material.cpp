#include "ui/Material.hpp"

namespace arterra {
	
	namespace UI {
		
		ShaderParameter::ShaderParameter(std::string name, Value value, Type type)
			: _name(name), _value(value), _type(type) {}
		
		Material::Material() 
			: _shaderIdentifier(""), _shaderParameters {} {}
		
		Material::Material(std::string shaderIdentifier) 
			: _shaderIdentifier(shaderIdentifier), _shaderParameters {} {
			
		}
		
		void Material::AddParameter(ShaderParameter shaderParameter) {
			_shaderParameters.push_back(shaderParameter);
		}
		
	}
	
}