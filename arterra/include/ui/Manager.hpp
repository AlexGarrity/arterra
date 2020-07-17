#pragma once

#include "PCH.hpp"
#include "ui/Element.hpp"
#include "renderer/GuiRenderer.hpp"

namespace arterra {
	
	namespace UI {
		
		class Manager {
			
			public:
				Manager(GuiRenderer* guiRenderer);
				
				void CreateElement(std::string identifier, Element element);
				void DestroyElement(std::string identifier);
				
				Element& GetElement(std::string identifier);
			
			private:
				std::unordered_map<std::string, Element> _elements;
				// Reference to gui renderer.
				GuiRenderer* _guiRenderer;
			
		};
		
	}
	
}