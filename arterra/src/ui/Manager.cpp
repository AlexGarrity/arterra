#include "ui/Manager.hpp"

namespace arterra {
	
	namespace UI {
		
		Manager::Manager(GuiRenderer* guiRenderer)
			: _elements(), _guiRenderer(guiRenderer) {}
		
		void Manager::CreateElement(std::string identifier, Element element) {
			// First run copy constructor.
			_elements[identifier] = element;
			
			// Then create the mesh on the "proper" copy.
			_elements[identifier].CreateMesh();
			_guiRenderer->AddElement(&_elements[identifier]);
		}
		
		void Manager::DestroyElement(std::string identifier) {
			// TODO: first delete element data
			auto element = _elements.find(identifier);
			if (element == _elements.end()) {
				Logger::Get().Log(Logger::Warning, "Trying to destroy UI element `", identifier,
					"' which does not exist!");
				return;
			}
			_elements.erase(element);
		}
		
		Element& Manager::GetElement(std::string identifier) {
			auto element = _elements.find(identifier);
			if (element == _elements.end()) {
				Logger::Get().Log(Logger::Warning, "Trying to access UI element `", identifier,
					"' which does not exist!");
			}
			return element->second;
		}
		
	}
	
}