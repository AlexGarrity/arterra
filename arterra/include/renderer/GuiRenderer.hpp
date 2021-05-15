#pragma once

#include "PCH.hpp"

#include "renderer/VertexArray.hpp"
#include "renderer/VertexBuffer.hpp"

namespace arterra {

	class Renderer;
	class GuiElement;
	
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