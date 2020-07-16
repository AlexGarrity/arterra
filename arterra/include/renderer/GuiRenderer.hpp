#pragma once

#include "PCH.hpp"

#include "renderer/Camera.hpp"
#include "renderer/Renderer.hpp"
#include "ui/GUIElement.hpp"

namespace arterra {
	
	class GuiRenderer {
		
		public:
			GuiRenderer(Renderer *renderer);
			~GuiRenderer();
			
			void AddElement(GuiElement* element);
			
			void Render();
			
			std::vector<GuiElement>& GetRenderables();
		
		private:
			std::vector<GuiElement*> _renderables;
			Renderer* _renderer;
			
	};
	
}