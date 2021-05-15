#pragma once

#include "PCH.hpp"

#include "renderer/VertexArray.hpp"
#include "renderer/VertexBuffer.hpp"

namespace arterra {

	class Renderer;
	class GuiElement;
	
	class GuiRenderer {
		
		public:
			GuiRenderer(const Renderer* renderer);
			~GuiRenderer();
			
			void AddElement(GuiElement* element);
			
			void Render();
			
			std::vector<GuiElement*>& GetRenderables();
		
		private:
			std::vector<GuiElement*> _renderables;
			const Renderer* _renderer;
			
	};
	
}