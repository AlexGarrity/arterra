#pragma once

#include "PCH.hpp"
#include "ui/Element.hpp"
#include "renderer/Renderer.hpp"
#include "renderer/ShaderManager.hpp"

namespace arterra {
	
	namespace UI {
		
		class Manager {
			
			public:
				Manager(ShaderManager* shaderManager, Renderer* renderer);
				
				void CreateElement(std::string identifier, Element element);
				void DestroyElement(std::string identifier);
				
				Element* GetElement(std::string identifier);
				inline std::unordered_map<std::string, Element>* GetElements() { return &_elements; }
				
				void Render();
				
			private:
				std::unordered_map<std::string, Element> _elements;
				
				ShaderManager* _shaderManager;
				Renderer* _renderer;
		};
		
	}
	
}