#pragma once

#include "PCH.hpp"

#include "renderer/Camera.hpp"
#include "renderer/Renderer.hpp"
#include "ui/Element.hpp"
#include "renderer/ShaderManager.hpp"

namespace arterra {
	
	class GuiRenderer {
		
		public:
			GuiRenderer(Renderer& renderer, ShaderManager& shaderManager);
			
			void AddElement(UI::Element* element);
			
			void Render();
			
		
		private:
			std::vector<UI::Element*> _renderables;
			Renderer* _renderer;
			
			ShaderManager* _shaderManager;
			
	};
	
}