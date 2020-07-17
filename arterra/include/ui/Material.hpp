#pragma once

#include "PCH.hpp"
#include <glm/glm.hpp>

namespace arterra {
	
	namespace UI {
		
		struct ShaderParameter {
			union Value {
				int i;
				float_t f;
				glm::vec2 v2;
				glm::vec3 v3;
				glm::vec4 v4;
				glm::mat4 m4;
				
				Value(glm::vec2 _v2) : v2(_v2) {};
				Value(glm::vec3 _v3) : v3(_v3) {};
				Value(glm::vec4 _v4) : v4(_v4) {};
				Value(glm::mat4 _m4) : m4(_m4) {};
				Value(float_t _f) : f(_f) {};
				Value(int _i) : i(_i) {};
				
			};
			enum class Type { Int, Float, Vec2, Vec3, Vec4, Mat4 };
			
			ShaderParameter(std::string name, Value value, Type type);
			
			std::string _name;
			Value _value;
			Type _type;
			
		};
		
		class Material {
			
			public:
				Material();
				Material(std::string shaderIdentifier);
				
				void AddParameter(ShaderParameter shaderParameter);
				void UpdateParameter(std::string name, glm::mat4 value);
				inline std::vector<ShaderParameter>& GetParameters() { return _shaderParameters; }
			
			private:
				// Name of the shader this material uses.
				std::string _shaderIdentifier;
				// List of uniforms for this shader.
				std::vector<ShaderParameter> _shaderParameters;
				
		};
		
	}
	
}