#include "renderer/GuiRenderer.hpp"

namespace arterra {
	
	GuiElement::GuiElement() {}
	
	GuiElement::GuiElement(std::vector<float_t> posVertices, std::vector<float_t> uvVertices) 
		: _posVertices(posVertices), _uvVertices(uvVertices) {
		GenerateMesh();
	}
	
	void GuiElement::Destroy() {
		_vertexArray.Destroy();
		_posBuffer.Destroy();
		_texBuffer.Destroy();
	}
	
	void GuiElement::GenerateMesh() {
		_posBuffer.Create(_posVertices, 2, GL_FLOAT);
		//_texBuffer.Create(_uvVertices, 2, GL_FLOAT);
		_vertexArray.AddBuffer(_posBuffer);
		//_vertexArray.AddBuffer(_texBuffer);
	}
	
	GuiRenderer::GuiRenderer(Renderer *renderer) {
		_renderer = renderer;
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
				std::cout << "drawing " << renderable->GetVertexCount() << " vertices" << std::endl;
				_renderer->DrawTriangles(renderable->GetVertexCount());
			}		
		}
		
	}
	
}