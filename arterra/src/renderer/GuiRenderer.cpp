#include "renderer/GuiRenderer.hpp"

#include "renderer/Renderer.hpp"
#include "renderer/GuiElement.hpp"

namespace arterra {

	GuiRenderer::GuiRenderer(const Renderer* renderer)
		: _renderer { renderer }
	{
	
	}
	
	GuiRenderer::~GuiRenderer() {
		for (auto renderable : _renderables) {
			renderable->Destroy();
		}
		_renderables.clear();
	}
	
	void GuiRenderer::AddElement(GuiElement* element) {
		_renderables.push_back(element);
	}
	
	void GuiRenderer::Render() {
		
		glDisable(GL_DEPTH_TEST);
		
		for (auto renderable : _renderables) {
			if (renderable->ShouldRender()) {
				renderable->Bind();
				_renderer->DrawTriangles(renderable->GetVertexCount());
			}		
		}
		
	}

	std::vector<GuiElement*>& GuiRenderer::GetRenderables()
	{
		return _renderables;
	}
	
}